Before start bench make sure that you have btrfs and xfs package

to begin test you have to create file block device with FS and prepare some steps, for example ext4:

mkdir /tmp/ext4_temp
dd if=/dev/zero of=temp_ext4 bs=1M count=100
mkfs.ext4 temp_ext4
sudo mount temp_ext4 /tmp/ext4_temp 
sudo chown username /tmp/ext4_temp

after that:

NF=1000 ./benchfs ext4