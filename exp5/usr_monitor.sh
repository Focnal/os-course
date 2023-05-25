if [[ $# -ne 1 ]];
then
    echo "usage: $0 <username>"
    echo "用法：$0 <用户名>"
    exit 1
fi

user=$1
logged_in_users=$(who | grep -w $user | awk '{print $1}')
logged_in_ports=$(who | grep -w $user | awk '{print $2}')
# 如果用户在多个位置上登录， 就挨个检查一遍
for logged_in_user in $logged_in_users
do
    if [[ $user == $logged_in_user ]];
    then
        echo -n "用户[$user]在[ "
        for logged_in_port in $logged_in_ports
        do
            echo -n "$logged_in_port "
        done 
        echo "]上登录"
        echo "按任意键退出"
        read -n1 -r -p ""
        exit 0
    fi
done

while true;
do
    echo "等待用户[$user]登录..."
    logged_in_users=$(who | grep -w $user | awk '{print $1}')
    logged_in_ports=$(who | grep -w $user | awk '{print $2}')
    # 如果用户在多个位置上登录， 就挨个检查一遍
    for logged_in_user in $logged_in_users
    do
        if [[ $user == $logged_in_user ]];
        then
            echo -n "用户[$user]在[ "
            for logged_in_port in $logged_in_ports
            do
                echo -n "$logged_in_port "
            done 
            echo "]上登录"
            echo "按任意键退出"
            read -n1 -r -p ""
            exit 0
        fi
    done

    sleep 1
done

echo "按任意键退出"
read -n1 -r -p ""