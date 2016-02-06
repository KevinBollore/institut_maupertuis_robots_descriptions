 [![Institut Maupertuis logo](https://avatars1.githubusercontent.com/u/12760694?v=3&s=80)](http://www.institutmaupertuis.fr) Institut Maupertuis robots descriptions
=======================================

Travis CI - [![Travis-CI](https://api.travis-ci.org/InstitutMaupertuis/institut_maupertuis_robots_descriptions.svg?branch=indigo-devel)](https://travis-ci.org/InstitutMaupertuis/institut_maupertuis_robots_descriptions/branches) 

# General information

This ROS package describes:
- Sensors
- End-effectors
- Work-cells
- Robots

<img src="https://raw.githubusercontent.com/InstitutMaupertuis/institut_maupertuis_robots_descriptions/indigo-devel/fanuc_r1000ia_grinding/fanuc_r1000ia_sls_2_grinding_support/r1000ia_grinding.png" align="center" height="200">
<img src="https://raw.githubusercontent.com/InstitutMaupertuis/institut_maupertuis_robots_descriptions/indigo-devel/fanuc_r1000ia_grinding/fanuc_r1000ia_ensenso_n10_grinding_support/r1000ia_grinding.png" align="center" height="200">
<img src="https://raw.githubusercontent.com/InstitutMaupertuis/institut_maupertuis_robots_descriptions/indigo-devel/fanuc_m10ia_kinect/fanuc_m10ia_kinect_support/m10ia_kinect.png" align="center" height="200">
<img src="https://raw.githubusercontent.com/InstitutMaupertuis/institut_maupertuis_robots_descriptions/indigo-devel/fanuc_m10ia_ensenso/fanuc_m10ia_ensenso_support/m10ia_ensenso.png" align="center" height="200">

Directories in the project
--------------------------

| Directory  | Description
------------ | -----------
`fanuc_m10ia_ensenso` | Fanuc M10iA with a Ensenso mounted on the tool
`fanuc_m10ia_kinect` | Fanuc M10iA with a Kinect mounted on the tool
`fanuc_r1000ia_grinding` | Fanuc R1000iA80f + Grinding end-effector + 3D sensor ( Ensenso or SLS-2) + grinding work-cell

| `sensors`: Directory  | Description
--------------------- | -----------
`ensenso_n10_description` | Ensenso N10
`kinect_v1_description` | Microsoft Kinect for Xbox 360 without the support
`sls_2_description` | David SLS-2

Dependencies
------------
- [Robot Operating System](http://wiki.ros.org/ROS/Installation)
- [`industrial-core`](http://wiki.ros.org/industrial_core)
- [`fanuc`](http://wiki.ros.org/fanuc)
- [`fanuc experimental`](http://wiki.ros.org/fanuc_experimental)

This package has been tested with Ubuntu 14.04 and ROS Indigo.

Testing the robots
------------------

`roslaunch fanuc_r1000ia_ensenso_n10_grinding_support test_r1000ia_grinding.launch`

`roslaunch fanuc_r1000ia_sls_2_grinding_support test_r1000ia_grinding.launch`
