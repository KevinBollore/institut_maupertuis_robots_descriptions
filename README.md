# Institut Maupertuis robots descriptions

![Institut Maupertuis logo](https://avatars1.githubusercontent.com/u/12760694?v=3&s=200)

This ROS package describes:
- Sensors
- End-effectors
- Work-cells
- Robots

<img src="https://raw.githubusercontent.com/InstitutMaupertuis/institut_maupertuis_robots_descriptions/indigo-devel/fanuc_r1000ia_sls_2_grinding_support/r1000ia_grinding.png" align="center" height="500">

<img src="https://raw.githubusercontent.com/InstitutMaupertuis/institut_maupertuis_robots_descriptions/indigo-devel/fanuc_r1000ia_ensenso_n10_grinding_support/r1000ia_grinding.png" align="center" height="500">

Directories in the project
--------------------------

| Directory  | Description
------------ | -----------
`fanuc_r1000ia_ensenso_n10_grinding_moveit_config` | `fanuc_r1000ia_ensenso_n10_grinding_support` MoveIt package
`fanuc_r1000ia_ensenso_n10_grinding_support` | Fanuc R1000iA80f + Grinding end-effector + Ensenso N10 + grinding work-cell
`fanuc_r1000ia_sls_2_grinding_moveit_config` | `fanuc_r1000ia_sls_2_grinding_support` MoveIt package
`fanuc_r1000ia_sls_2_grinding_support` | Fanuc R1000iA80f + Grinding end-effector + David SLS-2 + grinding work-cell
`grinding_effector_description` | Grinding end-effector
`grinding_workcell_description` | Grinding work-cell

| `sensors`: Directory  | Description
--------------------- | -----------
`ensenso_n10_description` | Ensenso N10
`sls_2_description` | David SLS-2

Dependencies
------------
- [Robot Operating System](http://wiki.ros.org/ROS/Installation)
- [`industrial-core`](https://github.com/ros-industrial/industrial_core)
- [`fanuc`](https://github.com/ros-industrial/fanuc)
- [`fanuc experimental`](https://github.com/ros-industrial/fanuc_experimental)

This package has been tested with Ubuntu 14.04 and ROS Indigo.

Testing the robots
------------------

`roslaunch fanuc_r1000ia_ensenso_n10_grinding_support test_r1000ia_grinding.launch`

`roslaunch fanuc_r1000ia_sls_2_grinding_support test_r1000ia_grinding.launch`
