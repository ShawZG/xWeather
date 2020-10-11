# XWeather
A crossed-platform weather application for desktop, built by Qt5 and Cmake.
![](./exhibition/city_weather.png)

#### 1. Build Environment
- Qt5(>5.9)
- CMake(>3.3)
  
#### 2. Build Step

##### 2.1 Install 
Assume that you are at the dir of project root.
```shell
1. mkdir build && cd build
2. cmake ..
3. make 
4. sudo make install
```

##### 2.2 Uninstall
Assume that you are at the build dir.
```shell
xargs rm < install_manifest.txt
```

#### 3. Build Deb packet
Assume that you are at the build dir.
```
1. cmake ..
2. cpack
```
Then, you will find a deb packet at build dir.