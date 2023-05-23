if [[ $# -ne 1 ]];
then
    echo "usage: $0 <username>"
    echo "用法：$0 <用户名>"
    exit 1
fi

user=$1

if [[ $user == $(who | grep -w $user | awk '{print $1}') ]];
then
    echo "用户[$user]已经登录！"
    echo "按任意键退出"
    read -n1 -r -p ""
    exit 0
fi


while true;
do
    echo "等待用户[$user]登录..."
    if [[ $user == $(who | grep -w $user | awk '{print $1}') ]];
    then
        echo "用户[$user]已经登录！"
    break
    fi
    sleep 1
done

echo "按任意键退出"
read -n1 -r -p ""