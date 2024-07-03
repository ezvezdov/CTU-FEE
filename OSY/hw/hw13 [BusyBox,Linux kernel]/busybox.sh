#!/bin/bash

# https://osy.pages.fel.cvut.cz/docs/cviceni/lab13/
# https://rtime.felk.cvut.cz/osp/cviceni/2/#index2h1

script_path="$(pwd)"

echo "Downloading busybox sources..."
git clone https://github.com/mirror/busybox

echo "Compiling busybox sources..."
cd busybox
make defconfig
make
cd ..

echo "Downloading linux kernel sources..."
# git clone https://github.com/torvalds/linux

echo "Compiling linux kernel sources..."
cd linux
# make defconfig
# make -j$(nproc)
cd ..

echo "Installing busybox..."
cd busybox
make install
cd ..

echo "Copying dependies.."
mkdir -p busybox/_install/lib
mkdir -p busybox/_install/lib64
deps=($(ldd busybox/_install/bin/busybox |  awk '{print $3}'))
cp ${deps[@]} busybox/_install/lib
cp /lib64/ld-linux-x86-64.so.2 busybox/_install/lib64
cp linux/usr/gen_init_cpio busybox/gen_init_cpio


echo "Creating RAM-disk..."
cd busybox/
(
cat <<EOF
dir /dev 755 0 0
nod /dev/tty0 644 0 0 c 4 0
nod /dev/tty1 644 0 0 c 4 1
nod /dev/tty2 644 0 0 c 4 2
nod /dev/tty3 644 0 0 c 4 3
nod /dev/tty4 644 0 0 c 4 4
slink /init bin/busybox 700 0 0
dir /proc 755 0 0
dir /sys 755 0 0
EOF

find _install -mindepth 1 -type d -printf "dir /%P %m 0 0\n"
find _install -type f -printf "file /%P %p %m 0 0\n"
find _install -type l -printf "slink /%P %l %m 0 0\n"
) > filelist
./gen_init_cpio filelist | gzip > ramdisk

echo "Creating startup script..."
mkdir -p _install/etc/init.d 
touch _install/etc/init.d/rcS 
cat <<EOF >_install/etc/init.d/rcS
#!/bin/sh
mount -t proc none /proc
echo -e "\nBoot took $(cut -d' ' -f1 /proc/uptime) seconds\n"
echo "Hello busybox!"
EOF
chmod +x _install/etc/init.d/rcS # make executable

rm -f gen_init_cpio filelist # clean trash

echo "Booting image..."
cd $script_path
qemu-system-x86_64 -kernel linux/arch/x86_64/boot/bzImage -initrd busybox/ramdisk


