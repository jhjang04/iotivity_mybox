script_dir=$(cd "$(dirname "$0")" && pwd)

cd $script_dir/../
git pull iotivity 1.3-rel
