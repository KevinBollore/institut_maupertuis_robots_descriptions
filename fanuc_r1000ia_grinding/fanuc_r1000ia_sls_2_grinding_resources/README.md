 [![Institut Maupertuis logo](https://avatars1.githubusercontent.com/u/12760694?v=3&s=80)](http://www.institutmaupertuis.fr) Institut Maupertuis robots descriptions
=======================================

# Run the calibration

To launch the robot + calibration service server:
```
roslaunch fanuc_r1000ia_sls_2_grinding_resources calibrate_sls_2_r1000ia_grinding.launch \
davidSDK_ip:=192.168.100.43 \
robot_ip:=192.168.100.200 \
sim:=false
```

In an other terminal, call the service:
```
rosservice call /calibration_service "allowable_cost_per_observation: 1.0"
```

# Test calibration

Make sure [yaml/mutable_joint_states.yaml](./yaml/mutable_joint_states.yaml) is updated with the new calibration matrix.

To launch the test:
```
roslaunch fanuc_r1000ia_sls_2_grinding_resources sls_2_calib_test.launch \
davidSDK_ip:=192.168.100.43 \
davidSDK_server_name:=m6700 \
robot_ip:=192.168.100.200 \
sim:=false
```
