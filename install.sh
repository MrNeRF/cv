!#/bin/sh

mkdir extern/ffmpeg/build
path=$(pwd)
cd extern/ffmpeg/build
../configure --prefix=$path --disable-doc --disable-programs
make -j8
make install
cd $path
rm -rf share
rm -rf extern/ffmpeg/build
