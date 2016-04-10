/*
 * processor_tracker.h
 *
 *  Created on: Apr 7, 2016
 *      Author: jvallve
 */

#ifndef SRC_PROCESSOR_TRACKER_H_
#define SRC_PROCESSOR_TRACKER_H_

#include "processor_base.h"
#include "capture_base.h"

/** \brief Feature tracker processor
 *
 * This is an abstract class.
 *
 * This class implements the incremental feature tracker.
 *
 * The incremental tracker contains three pointers to three Captures of type CaptureBase,
 * named \b origin, \b last and \b incoming:
 *   - \b origin: this points to a Capture where all Feature tracks start.
 *   - \b last: the last Capture tracked by the tracker.
 *     A sufficient subset of the Features in \b origin is still alive in \b last.
 *   - \b incoming: the capture being received. The tracker operates on this Capture,
 *     establishing correspondences between the features here and the features in \b origin.
 *     Each successful correspondence
 *     results in an extension of the track of the Feature up to the \b incoming Capture.
 *
 * It establishes constraints Feature-Feature or Feature-Landmark;
 * Implement these options in two separate derived classes:
 *   - ProcessorTrackerFeature : for Feature-Feature correspondences (no landmarks)
 *   - ProcessorTrackerLandmark : for Feature-Landmark correspondences (with landmarks)
 *
 * The pipeline of actions for an autonomous tracker can be resumed as follows:
 *   - Init the tracker with an \b origin Capture: init();
 *   - On each incoming Capture,
 *     - Track known features in the \b incoming Capture: processKnownFeatures();
 *       - For each tracked Feature:
 *          - create constraints: createConstraint()
 *     - Check if enough Features are still tracked, and vote for a new KeyFrame if this number is too low:
 *     - if voteForKeyFrame()
 *       - Populate the tracker with new Features : processNew()
 *       - detectNewFeatures()
 *       - Make a KeyFrame with the \b last Capture: makeKeyFrame();
 *       - Reset the tracker with the \b last Capture as the new \b origin: reset();
 *     - else
 *       - Advance the tracker one Capture ahead: advance()
 *
 * This functionality exists by default in the virtual method process(). You can overload it at your convenience.
 */
class ProcessorTracker : public ProcessorBase
{
    protected:
        CaptureBase* origin_ptr_;    ///< Pointer to the origin of the tracker.
        CaptureBase* last_ptr_;      ///< Pointer to the last tracked capture.
        CaptureBase* incoming_ptr_;  ///< Pointer to the incoming capture being processed.
        FeatureBaseList new_features_last_; ///< List of new features in \b last for landmark initialization and new key-frame creation.
        FeatureBaseList new_features_incoming_; ///< list of the new features of \b last successfully tracked in \b incoming

    public:
        ProcessorTracker(ProcessorType _tp);
        virtual ~ProcessorTracker();

        /** \brief Initialize tracker.
         * This function accepts a Capture to be used as the origin.
         * This Capture is required to be already attached to a KeyFrame in WolfProblem.
         */
        void init(CaptureBase* _origin_ptr);

        /** \brief Full processing of an incoming Capture.
         *
         * Usually you do not need to overload this method in derived classes.
         * Overload it only if you want to alter this algorithm.
         */
        virtual void process(CaptureBase* const _incoming_ptr);

        CaptureBase* getLastPtr();

    protected:

        /** \brief Tracker function
         * \return The number of successful tracks.
         *
         * This is the tracker function to be implemented in derived classes.
         * It operates on the \b incoming capture pointed by incoming_ptr_.
         *
         * This should do one of the following, depending on the design of the tracker -- see use_landmarks_:
         *   - Track Features against other Features in the \b origin Capture. Tips:
         *     - An intermediary step of matching against Features in the \b last Capture makes tracking easier.
         *     - Once tracked against last, then the link to Features in \b origin is provided by the Features' Constraints in \b last.
         *     - If required, correct the drift by re-comparing against the Features in origin.
         *     - The Constraints in \b last need to be transferred to \b incoming (moved, not copied).
         *   - Track Features against Landmarks in the Map. Tips:
         *     - The links to the Landmarks are in the Features' Constraints in last.
         *     - The Constraints in \b last need to be transferred to \b incoming (moved, not copied).
         *
         * The function must generate the necessary Features in the \b incoming Capture,
         * of the correct type, derived from FeatureBase.
         *
         * It must also generate the constraints, of the correct type, derived from ConstraintBase
         * (through ConstraintAnalytic or ConstraintSparse).
         */
        virtual unsigned int processKnown() = 0;

        /** \brief Vote for KeyFrame generation
         *
         * If a KeyFrame criterion is validated, this function returns true,
         * meaning that it wants to create a KeyFrame at the \b last Capture.
         *
         * WARNING! This function only votes! It does not create KeyFrames!
         */
        virtual bool voteForKeyFrame() = 0;

        /** \brief Advance the incoming Capture to become the last.
         *
         * Call this when the tracking and keyframe policy work is done and
         * we need to get ready to accept a new incoming Capture.
         */
        virtual void advance();

        /**\brief Process new Features or Landmarks
         *
         */
        virtual unsigned int processNew() = 0;

        /** \brief Detect new Features in the \b last Capture
         * \return The number of detected Features.
         *
         * This function detects Features that do not correspond to known Features/Landmarks in the system.
         *
         * The function sets the member new_features_list_last_, the list of newly detected features.
         */
        virtual unsigned int detectNewFeatures() = 0;

        /**\brief Creates and adds constraints from last_ to origin_
         *
         */
        virtual void establishConstraints()=0;

        /**\brief make a non-key Frame with the provided Capture
         */
        void makeFrame(CaptureBase* _capture_ptr);

        /**\brief Make a non-key frame at \b incoming and set KeyFrame at \b last
         */
        virtual void makeKeyFrame();

        /** \brief Reset the tracker using the \b last Capture as the new \b origin.
         */
        void reset();

    protected:

        FeatureBaseList& getNewFeaturesListLast();

        void addNewFeatureLast(FeatureBase* _feature_ptr);

        FeatureBaseList& getNewFeaturesListIncoming();

        void addNewFeatureIncoming(FeatureBase* _feature_ptr);
};

inline void ProcessorTracker::init(CaptureBase* _origin_ptr)
{
    origin_ptr_ = _origin_ptr;
    last_ptr_ = _origin_ptr;
    detectNewFeatures(); // This operates on last but it's equal to origin.
    last_ptr_->addDownNodeList(new_features_last_);
}

inline void ProcessorTracker::makeFrame(CaptureBase* _capture_ptr)
{
    // We need to create the new free Frame to hold what will become the last Capture
    FrameBase* new_frame_ptr = getWolfProblem()->createFrame(NON_KEY_FRAME, _capture_ptr->getTimeStamp());
    new_frame_ptr->addCapture(_capture_ptr); // Add incoming Capture to the new Frame
    new_frame_ptr->setTimeStamp(_capture_ptr->getTimeStamp());
}

inline void ProcessorTracker::makeKeyFrame()
{
    // Create a new non-key Frame in the Trajectory with the incoming Capture
    makeFrame(incoming_ptr_);
    // Make the last Capture's Frame a KeyFrame so that it gets into the solver
    last_ptr_->getFramePtr()->setKey();

    // Create constraints from last to origin
    establishConstraints();
}

inline void ProcessorTracker::reset()
{
    origin_ptr_ = last_ptr_;
    last_ptr_ = incoming_ptr_;
    incoming_ptr_ = nullptr; // This line is not really needed, but it makes things clearer.
}

inline FeatureBaseList& ProcessorTracker::getNewFeaturesListLast()
{
    return new_features_last_;
}

inline void ProcessorTracker::addNewFeatureLast(FeatureBase* _feature_ptr)
{
    new_features_last_.push_back(_feature_ptr);
}

inline FeatureBaseList& ProcessorTracker::getNewFeaturesListIncoming()
{
    return new_features_incoming_;
}

inline void ProcessorTracker::addNewFeatureIncoming(FeatureBase* _feature_ptr)
{
    new_features_incoming_.push_back(_feature_ptr);
}

inline CaptureBase* ProcessorTracker::getLastPtr()
{
    return last_ptr_;
}

#endif /* SRC_PROCESSOR_TRACKER_H_ */
