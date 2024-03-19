#!/bin/bash

# 定义开发板的SSH登录信息
DEVELOP_BOARD_USER="compilerhit"
DEVELOP_BOARD_IP="10.249.62.124"

LOCAL_USER="wangfiox"
LOCAL_IP="10.249.88.161"

# 定义笔记本上的共享目录和开发板上的挂载点
LOCAL_SHARE_DIR="/home/wangfiox/Documents/rv64gc-thrd"  # 笔记本上需要被共享的目录路径
REMOTE_MOUNT_POINT="/home/compilerhit/wangfiox/"  # 开发板上的挂载点路径

# SSH到开发板并执行sshfs命令来挂载远程目录
ssh $DEVELOP_BOARD_USER@$DEVELOP_BOARD_IP <<EOF
sshpass -p "password" sshfs $LOCAL_USER@$LOCAL_IP:$LOCAL_SHARE_DIR $REMOTE_MOUNT_POINT
EOF

echo "ok."
