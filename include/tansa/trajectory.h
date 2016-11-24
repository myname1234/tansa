#ifndef TANSA_TRAJECTORY_H_
#define TANSA_TRAJECTORY_H_

#include <Eigen/Dense>
#include <vector>

using namespace Eigen;
using namespace std;


namespace tansa {

const unsigned PointDims = 3;
// TODO: Should eventually be Vector4d to incorporate yaw
typedef Vector3d Point;


// Used for determining feasibility of trajectories
#define MAX_ACCELERATION 3


/**
 * Evaluation of a trajectory at a point in time
 */
struct TrajectoryState {
	Point position;
	Point velocity;
	Point acceleration;
};


/**
 * A path that the vehicle should follow constrained to a given time period
 * They are parametrized w.r.t. time
 * Should be at least a three time differentiable function
 */
class Trajectory {
public:
	Trajectory(double t1, double t2) {
		this->t1 = t1;
		this->t2 = t2;
	}
	virtual ~Trajectory() {};
	virtual TrajectoryState evaluate(double t) = 0;

	inline double startTime() { return this->t1; }
	inline double endTime() { return this->t2; }

protected:

	double t1, t2;

};

/**
 * Concatenation of many temporally offset trajectories
 */
class PiecewiseTrajectory : public Trajectory {
public:


};

class PolynomialTrajectory : public Trajectory {
public:

	PolynomialTrajectory(const VectorXd c[], double t1, double t2);
	virtual ~PolynomialTrajectory(){}
	/**
	 * Computes an 'optimal' polynomial trajectory between two times given some constraints on the derivatives of the start and end points
	 */
	static PolynomialTrajectory *compute(const vector<Point> &c1, double t1, const vector<Point> &c2, double t2);


	virtual TrajectoryState evaluate(double t);

private:

	// Store coefficients for x, y, z
	VectorXd coeffs[PointDims];
};

/**
 * Smoothly goes in a straight line through two points
 */
class LinearTrajectory : public Trajectory {
public:

	LinearTrajectory(Point x1, double t1, Point x2, double t2);
	virtual ~LinearTrajectory();

	virtual TrajectoryState evaluate(double t);

private:

	PolynomialTrajectory *inner;


};

/**
 * A 2d circle in the XY plane. An angle of 0 is on the X axis
 */
class CircleTrajectory : public Trajectory {
public:
	CircleTrajectory(const Point &origin, double radius, double theta1, double t1, double theta2, double t2);
	virtual ~CircleTrajectory(){}
	virtual TrajectoryState evaluate(double t);


private:
	Point origin;
	double radius;
	double theta1;
	double dtheta;

};

/**
 * Stays at one point. Mainly just for testing.
 */
class PointTrajectory : public Trajectory {
public:
	PointTrajectory(const Point &p);
	virtual ~PointTrajectory(){}
	virtual TrajectoryState evaluate(double t);

private:
	Point p;

};

/**
 * Smoothly increases or decreases list intensity between two intensities
 */
class LightTrajectory {
public:

	inline LightTrajectory(double si, double st, double ei, double et) :
			startTime(st), endTime(et), startIntensity(si), endIntensity(ei) {}
	virtual ~LightTrajectory() {}

	// Gives the intensity at a given time between the start and end times
	virtual double evaluate(double t);

	inline double getStartTime() { return this->startTime; }
	inline double getEndTime() { return this->endTime; }

	inline double getStartIntensity() { return this->startIntensity; }
	inline double getEndIntensity() { return this->endIntensity; }

protected:

	double startTime, endTime, startIntensity, endIntensity;
};

/**
 * Smoothly increases or decreases list intensity between two intensities
 */
class StrobeTrajectory : public LightTrajectory {
public:

	inline StrobeTrajectory(double si, double st, double ei, double et, double bpm) :
			secondsPerBeat(bpm), LightTrajectory(si,st,ei,et) {}
	virtual ~StrobeTrajectory() {}

	// Gives the intensity at a given time between the start and end times
	virtual double evaluate(double t);

private:

	double secondsPerBeat;
};


/**
 * Does a spiral in the z direction. Implemented as an normalized circle with a moving center trajectory
 */
class SpiralTrajectory : public Trajectory {
public:


	virtual TrajectoryState evaluate(double t);


private:
	LinearTrajectory *center;
	CircleTrajectory *circle;
};


}

#endif
