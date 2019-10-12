## Feedback for Assignment 02

Run on October 11, 22:00:18 PM.


### Necessary Files and Structure

+ :heavy_check_mark:  Check that directory "dev" exists.

+ :heavy_check_mark:  Copy directory "Files for Testing".



+ :heavy_check_mark:  Check that directory "dev/MinGfx" exists.


### Worksheet Tests

+ :heavy_check_mark:  Check that file "worksheets/a2_carsoccer.md" exists.


### Compile Tests


#### Car Soccer

+ :heavy_check_mark:  Check that directory "dev/a2-carsoccer" exists.

+ :heavy_check_mark:  Change into directory "dev/a2-carsoccer".

+ :heavy_check_mark:  Configuring assignment for grading.



+ :heavy_check_mark:  Make directory "build".

+ :heavy_check_mark:  Change into directory "build".

+ :heavy_check_mark:  Check that CMake Configures.

+ :x:  Check that make compiles.

    Make compile fails with errors:.
<pre>Scanning dependencies of target a2-carsoccer
[ 16%] Building CXX object CMakeFiles/a2-carsoccer.dir/car_soccer.cc.o
In file included from /project/grades/Fall-2019/csci4611/student-repos/Assignment_02_Feedback/repo-wei00120/dev/a2-carsoccer/car_soccer.h:11:0,
                 from /project/grades/Fall-2019/csci4611/student-repos/Assignment_02_Feedback/repo-wei00120/dev/a2-carsoccer/car_soccer.cc:4:
/project/grades/Fall-2019/csci4611/student-repos/Assignment_02_Feedback/repo-wei00120/dev/a2-carsoccer/car.h:48:1: error: version control conflict marker in file
 <<<<<<< HEAD
 ^~~~~~~
/project/grades/Fall-2019/csci4611/student-repos/Assignment_02_Feedback/repo-wei00120/dev/a2-carsoccer/car_soccer.cc: In member function virtual void CarSoccer::UpdateSimulation(double):
/project/grades/Fall-2019/csci4611/student-repos/Assignment_02_Feedback/repo-wei00120/dev/a2-carsoccer/car_soccer.cc:152:10: error: class Car has no member named move
     car_.move(timeStep);
          ^~~~
CMakeFiles/a2-carsoccer.dir/build.make:62: recipe for target 'CMakeFiles/a2-carsoccer.dir/car_soccer.cc.o' failed
make[2]: *** [CMakeFiles/a2-carsoccer.dir/car_soccer.cc.o] Error 1
CMakeFiles/Makefile2:67: recipe for target 'CMakeFiles/a2-carsoccer.dir/all' failed
make[1]: *** [CMakeFiles/a2-carsoccer.dir/all] Error 2
Makefile:129: recipe for target 'all' failed
make: *** [all] Error 2
</pre>



+ :leftwards_arrow_with_hook:  Check that a GoogleTest test passes. (Test not run because of an earlier failing test)

