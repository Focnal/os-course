clear
# 命令行启动
# 判断参数数量是否正确
if [[ ($# -ne 1 && $# -ne 2 && $# -ne 0 ) || ($# -eq 2 && !($2 =~ ^[0-9]+$)) ]]; 
then
    echo "使用方法: $0 <文件路径> [检测间隔（秒）]"
    exit 1
fi

file_path=

if [ $# -eq 0 ];
then
    echo "请输入文件名"
    read -p ">>" file_path
else
    file_path=$1
fi
# 检查文件有效性
if [ ! -e $file_path ];
then
    echo "文件不存在！"
    exit 1
fi

prev_size=$(ls -l $file_path | awk '{print $5}') #当然也可以用 $(du -b $file_path | cut -f1)
change_round=0
no_change_round=0
max_change_round=2
max_no_change_round=10
if [ $# -eq 2 ]; then
    sleep_interval=$2
else
    sleep_interval=1
fi

echo "开始对 $file_path 文件大小的检测"

while true; 
do
    sleep $sleep_interval
    current_size=$(ls -l $file_path | awk '{print $5}')

    # 检查文件大小是否发生变化
    if [ "$current_size" -ne "$prev_size" ]; 
    then
        echo "文件大小发生变化，从$prev_size变为$current_size"  # 如果文件大小发生变化就将no_change_round重置为0
        prev_size=$current_size
        change_round=$((change_round + 1))
        no_change_round=0 
    else
        no_change_round=$((no_change_round + 1))
        echo "文件大小没有发生变化"
    fi

    # 检查是否达到最大变化次数
    if [ "$change_round" -ge "$max_change_round" ]; 
    then
        echo "文件大小变化次数达到$max_change_round次，停止检测。"
        break
    fi

    # 检查是否到达最大无变化次数
    if [ "$no_change_round" -ge "$max_no_change_round" ]; 
    then
        echo "连续$max_no_change_round次检测文件大小未发生变化，停止检测。"
        break
    fi
done
echo "按任意键退出（不是关机键）"
read -n1 -r -p ""

clear
