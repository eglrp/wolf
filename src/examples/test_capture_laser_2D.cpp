//wolf
#include "capture_laser_2D.h"

//main
int main(int argc, char *argv[])
{
    std::cout << std::endl << "CaptureLaser2D class test" << std::endl;
    std::cout << "========================================================" << std::endl;
    
    //scan ranges
    Eigen::VectorXs ranges(720);
    ranges << 	2.78886,2.78289,2.7832,2.78367,2.7843,2.78508,2.78603,2.78713,2.78839,2.78981,
		2.79139,2.78669,2.78855,2.79057,2.79274,2.79507,2.79098,2.79359,2.79635,2.79927,
		2.79566,2.79886,2.80221,2.79895,2.80258,2.80638,2.80345,2.80753,2.81176,2.80917,
		2.81368,2.81132,2.81611,2.81396,2.81903,2.73664,2.72743,2.71782,2.70883,2.69945,
		2.69067,2.6815,2.67294,2.66398,2.65562,2.64736,2.6387,2.63064,2.62218,2.61431,
		2.60605,2.59837,2.59078,2.5828,2.57539,2.56759,2.56036,2.55321,2.54568,2.53871,
		2.53182,2.52455,2.51782,2.51118,2.50415,2.49767,2.49127,2.48495,2.47824,2.47207,
		2.46597,2.4595,2.45355,2.44768,2.44188,2.4357,2.43005,2.42446,2.41894,2.4135,
		2.40768,2.40236,2.39712,2.39195,2.3864,2.38135,2.37637,2.37146,2.36661,2.36182,
		2.35666,2.352,2.3474,2.34286,2.34186,2.36289,2.41051,2.43311,2.45628,2.48003,
		2.50439,2.52937,2.555,2.61224,2.63993,2.66837,2.69757,2.72758,2.75841,2.79011,
		2.82269,2.85621,2.8907,2.96659,3.0042,3.04295,3.08289,3.12406,3.16652,3.21033,
		3.22693,3.23444,3.24207,3.24982,3.2577,3.26571,3.2855,3.29383,3.30229,3.31088,
		3.3196,3.32846,3.33745,3.34658,3.35584,3.36524,3.37478,3.38445,3.39427,3.40423,
		3.41434,3.42459,3.43499,3.44553,3.45623,3.46707,3.47807,3.48923,3.50053,3.512,
		3.52362,3.53541,3.54736,3.55947,3.57175,3.5842,3.59681,3.6096,3.62256,3.63569,
		3.64901,3.6625,3.67617,3.69003,3.70407,3.7183,3.73272,3.74733,3.76214,3.7931,
		3.80843,3.82397,3.83972,3.85567,3.87183,3.88822,3.90481,3.92163,3.93867,3.95593,
		3.97343,3.97347,3.99127,4.00931,4.02758,4.0461,4.06486,4.08387,4.10314,4.12266,
		4.14244,4.16248,4.20237,4.22313,4.24418,4.26551,4.28712,4.30903,4.33122,4.35372,
		4.37652,4.39963,4.42304,4.44678,4.47084,4.49523,4.51994,4.545,4.57041,4.59615,
		4.62225,4.64871,4.67554,4.70274,4.73032,4.75828,4.78663,4.81538,4.84452,4.8741,
		4.90409,4.9345,4.96534,4.99662,5.02836,5.06053,5.09317,5.12628,5.15987,5.19395,
		5.22853,5.2636,5.2992,5.33532,5.37197,5.44106,5.47923,5.51796,5.55729,5.59721,
		5.63773,5.67888,5.72066,5.76307,5.80615,5.8499,5.89432,5.93945,6.02374,6.07085,
		6.11872,6.16734,6.21676,6.26698,6.318,6.36987,6.42258,6.47618,6.5758,6.6319,
		6.68894,6.74694,6.80593,6.86593,6.92698,7.04022,7.10425,7.1694,7.23572,7.30322,
		7.37192,7.49928,7.57149,7.64505,7.58372,7.51951,7.45681,7.32129,7.32938,7.34276,
		7.35632,7.36877,7.38272,7.39687,7.41124,7.4258,7.43923,7.4542,7.46937,7.48477,
		7.49904,7.51485,7.53087,7.54579,7.56225,7.57894,7.59587,7.61164,7.62902,8.37389,
		8.39194,8.41173,8.43177,8.45059,8.47118,8.49202,8.51164,8.53305,8.55319,8.57516,
		8.59739,8.61839,8.64122,8.6628,8.6862,8.70837,8.73237,8.7567,8.77979,8.80472,
		8.82843,8.85402,8.87835,8.9046,8.9296,8.9565,8.98218,9.0082,9.03616,9.06287,
		9.09152,9.11895,9.14834,9.17652,9.20503,9.23557,9.26487,9.29454,9.32626,9.35674,
		9.38762,9.42055,9.45226,9.4844,9.51695,9.55162,9.58503,9.61893,9.65324,4.38929,
		4.38536,4.36058,4.3365,4.3131,4.29036,4.26827,4.24682,4.22598,4.20576,4.18612,
		4.1944,4.17582,4.15708,4.13859,4.12032,4.10229,4.08449,4.06691,4.04955,4.03241,
		4.01549,3.99916,3.98265,3.96634,3.95024,3.93434,3.91901,3.90349,3.88817,3.87304,
		3.85845,3.84368,3.82909,3.81504,3.80081,3.78674,3.7732,3.75948,3.74591,3.73287,
		3.71963,3.7069,3.69398,3.68156,3.66894,3.65647,3.6445,3.63233,3.62065,3.60876,
		3.59736,3.58576,3.58265,3.61553,3.62696,3.63867,3.67347,3.68596,3.72229,3.7356,
		3.77355,3.78772,3.80219,3.84244,3.85785,3.89993,3.9163,3.93303,3.97774,3.99551,
		4.01367,4.06121,4.0805,4.10019,4.15081,4.17174,4.19309,13.8062,13.7714,13.7384,
		13.7075,13.8936,13.9735,14.0549,14.1382,14.3407,15.8017,15.9009,16.002,16.1054,
		16.3519,16.462,16.5744,16.6893,16.9594,17.0819,17.2072,17.3352,17.4661,17.6,
		8.14878,8.1334,8.11823,8.10324,8.08848,8.07391,8.0588,8.04465,8.03069,8.01693,
		8.00338,7.99,7.97684,7.96312,7.95032,7.93773,7.92533,7.91309,7.90106,7.88922,7.87755,
		7.86606,7.85476,7.84289,7.83195,7.82116,7.81058,7.80017,7.78993,7.77984,7.76995,
		7.76021,7.75066,7.74128,7.73204,7.76034,7.99805,8.11853,8.24311,8.37202,12.3718,
		12.3587,12.346,12.3336,12.3213,12.3094,12.2976,12.2862,12.275,12.264,12.2534,12.2429,
		12.2327,12.2228,12.213,12.2036,12.1944,12.1854,12.1766,12.3577,12.667,16.7608,16.7501,
		16.7398,16.7297,16.7201,16.7106,16.7015,16.6929,16.6844,16.9488,20.069,20.0619,20.0552,
		20.0489,20.043,20.0374,20.0323,20.0275,20.0231,20.0191,20.0155,20.0122,20.0094,20.0069,
		20.0048,20.0031,20.0018,20.0008,20.0002,20.0001,20.0002,20.0008,20.0018,20.0031,20.0048,
		20.0069,20.0094,20.0122,20.0155,20.0191,20.0231,20.0275,20.0323,20.0374,20.043,20.0489,
		20.0552,20.0619,20.069,20.0764,20.0843,20.0926,20.1012,20.1102,20.1196,20.1294,20.1397,
		20.1502,20.1612,20.1726,20.1844,20.1966,20.2092,20.2222,20.2356,20.2494,20.2636,20.2782,
		20.2932,20.3086,20.3244,20.3407,20.3573,20.3744,20.3919,20.4098,20.4281,20.4469,20.466,
		20.4856,20.5057,20.5261,20.547,20.5684,20.5901,20.6123,20.635,20.6581,20.6816,20.7056,
		20.73,20.7549,20.7802,20.806,20.8323,20.859,20.8862,20.9139,20.942,20.9706,20.9997,
		21.0293,21.0594,21.0899,21.1209,21.1525,21.1845,21.217,21.2501,21.2836,21.3177,21.3522,
		21.3873,21.423,21.4591,21.4958,21.533,21.5707,21.6091,21.6479,21.6873,21.7273,21.7678,
		21.8089,21.8505,21.8928,21.9356,21.979,22.023,22.0676,22.1128,22.1586,22.2051,22.2521,
		22.2998,22.3481,22.397,22.4466,22.4968,22.5477,22.5992,22.6515,22.7043,22.7579,22.8122,
		22.8671,22.9228,22.9792,23.0363,23.0941,23.1526,23.2119,23.2719,23.3327,23.3943,23.4566,
		23.5197,23.5836,23.6483,23.7138,23.7802,23.8473,23.9153,23.9842,24.0539,24.1244,24.1959,
		24.2682,24.3414,24.4156,24.4906,24.5666,24.6435,24.7214,24.8003,24.8801,24.9609,25.0428,
		25.1256,25.2095,25.2944,25.3804,25.4675,25.5556,25.6449,25.7353,25.8268,25.9194,26.0132,
		26.1082,26.2044,26.3018,26.4004,26.5003,26.6015,26.7039,26.8077,26.9127,27.0191,27.1269,
		27.2361,27.3466,27.4586,27.572,27.6869,27.8033,27.9213,28.0407,28.1617;
    
    //variable declarations and inits
    Eigen::VectorXs device_pose(6);
    device_pose << 0,0,0,0,0,0; //origin, no rotation
    TimeStamp time_stamp;
    time_stamp.setToNow();
    std::list<Eigen::Vector2s> corner_list;
    std::list<Eigen::Vector2s>::iterator corner_it;
    
    //Device and Capture declaration
    SensorLaser2D device(device_pose, ranges.size(), M_PI, 0.2, 30.0, 0.01);
    CaptureLaser2D capture(time_stamp, &device, ranges);
    
    //do things with the measurements
    capture.extractCorners(corner_list);

    //print corners
    std::cout << "CORNER LIST" << std::endl;            
    for ( corner_it = corner_list.begin(); corner_it != corner_list.end(); corner_it ++ )
    {
        std::cout << (*corner_it).x() << " , " << (*corner_it).y() << std::endl;
    }
    
    std::cout << "========================================================" << std::endl;            
    std::cout << std::endl << "End CaptureLaser2D class test" << std::endl;
    return 0;
}

