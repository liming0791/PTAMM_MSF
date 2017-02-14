adb pull /sdcard/gmon.out ./$1
/home/liming/Projects/android-ndk-r13b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gprof /home/liming/APTAM-GPL/APTAM/obj/local/armeabi-v7a/libPTAM.so $1 > $2
cat $2 | /home/liming/Projects/gprof2dot/gprof2dot.py | dot -Tpng -o $3
