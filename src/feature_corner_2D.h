#ifndef FEATURE_CORNER_2D_H_
#define FEATURE_CORNER_2D_H_

//std includes

//Wolf includes
#include "feature_base.h"
//#include "constraint_corner_2D.h"

//class FeatureCorner2D
class FeatureCorner2D : public FeatureBase
{
    public:
        /** \brief Constructor 
         * 
         * constructor
         */
        FeatureCorner2D(const Eigen::Vector4s & _measurement, const Eigen::Matrix4s & _meas_covariance);

        /** \brief Destructor 
         * 
         * destructor
         *
         */        
        virtual ~FeatureCorner2D();

        //TODO: Encara que sigui brutforce jo la búsqueda la faria a nivell capture.
        /** \brief Generic interface to find constraints
         * 
         * Generic interface to find constraints between this feature and a map (static/slam) or a previous feature
         *
         **/
        //virtual void findConstraints();
        
};
#endif