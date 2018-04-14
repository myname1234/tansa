Tansa: Dancing Robotics Platform
================================
[![Build Status](https://travis-ci.org/dennisss/tansa.svg?branch=master)](https://travis-ci.org/dennisss/tansa)

<link rel="stylesheet" type="text/css" href="https://stackpath.bootstrapcdn.com/bootstrap/4.1.0/css/bootstrap.min.css" >

An open platform for making drones dance!

| We designed and built a 260mm quadcopter with 2 RGB spot lights and 20+ min battery life  | We have smooth transitions and minimum snap trajectories between waypoints | The controllers are optimized for high speed maneuvers
|-|-|-|
| ![](https://storage.googleapis.com/tansa/imgs/dronecad.png) | ![](https://storage.googleapis.com/tansa/imgs/minsnap.gif) | ![](https://storage.googleapis.com/tansa/imgs/fastsnippets.gif) |


| We can seamlessly fly a whole swarm (up to 6 over WiFi) | Did I mention lights? |  Before we break our drones, we always test using our web based GUI
|-|-|-|
| ![](https://storage.googleapis.com/tansa/imgs/coordinatedcircle.gif) | ![](https://storage.googleapis.com/tansa/imgs/coordinatedlights.gif) | ![](https://storage.googleapis.com/tansa/imgs/gui.gif) |





Overview
--------

This project includes the following:


1. Reference quadcopter electrical/mechanical designs used for developing the system
	- Around $500 BOM cost. Requires a drill, saw, 3d printer, off the shelf parts, and time to replicate.
	- Features 2 controllable spotlights for theatrical purposes
	- See `hardware/` directory

2. Drone control algorithms and drivers
	- Written in C++ with no dependency on ROS
	- Interface to networked MAVLink vehicles
	- Trajectory generation and control
		- Control based on the paper: `Minimum Snap Trajectory Generation and Control for Quadrotors` by Daniel Mellinger and Vijay Kumar
	- Motion capture support
		- Driver for using with OptiTrack Motive 1.9.0
	- Simulation Environment Integration
		- All simulation integrations are multi-drone capable out of the box
		- Gazebo support via PX4/sitl_gazebo

3. Operator User Interface
	- HTML/JS interface based for visualizing and running scripts

License
-------

Unless otherwise specified, all current files and future contributions in this repository are distributed under the MIT License (see `LICENSE` file).


Requirements
------------

This platform was primary developed for UNIX based OSes (Linux / Mac OS X), but should be mostly compatible with Windows as well (probably won't compile on windows in it's current state though).

You can optionally skip most of this and try using one of your prebuilt Docker images (see `config/docker/` directory).

For running/compiling the core code, the following libraries must be installed:

- CMake
- Eigen3
- Boost
- CGAL

For running the simulations, all dependencies on the PX4 toolchain/gazebo SITL environment must be met:
- See http://dev.px4.io/starting-installing.html
- See http://dev.px4.io/simulation-gazebo.html
	- We test with Gazebo 7

UI Dependencies
- Node.js >6.0.0
- A recent web browser such as Google Chrome that supports WebGL

Recommended/Optional Dependencies:
- The IBM CPLEX library is a recommended addition if you are using advanced trajectory generation. It is required for making collision free trajectories. If not included, it will fallback to the less stable CGAL solvers when possible.


Building
--------

1. Run `make build` to compile all the core libraries

2. Run `make build_firmware` to build the nested PX4 code and all code needed for Gazebo simulation

3. Run `npm install` to download packages needed for the GUI

Alternatively, run `make sim` to start up a multidrone simulator

Run `make run` to build the core stuff and start up the controller. Make sure that config.json has the right filepaths, and the right ip addresses if not using simulator.


Running
-------

Once everything is built, you can run `./start.sh` to quick start into a simulator with the UI running.

The script does three things

1. Starts the main gcs program via `make run`
	- This communicates and controls the drones

2. Starts the simulator using `make runSim`

3. Serves the UI via `make server`


UI
--

Viewable at `http://127.0.0.1:4000` after starting the server

- The root url will take you to the web-based 3d viewer and control suite

- Each 'choreography' is available to load and correspondences to files in the `data/` folder

- Upon loading a file, the drones should automatically spawn into the environment


Documentation
-------------

See the `doc` folder

A full source code documentation page can be generated by running `make doc`. Then the documentation can be viewed from `/build_doc/html/index.html`
