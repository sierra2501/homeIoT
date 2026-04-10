#!/bin/sh

echo "Receive BLE Data"

# ============
# GATT
# ============
GATT_RAW=$(gatttool -b XX:XX:XX:XX:XX:XX --char-read --handle=0x002a)
GATT=$(echo ${GATT_RAW##*: } | for n in `xargs`;do echo "$((0x$n))" | awk '{printf "%c",$1}'; done;echo)

# ============
# NULL判定
# ============
if [ -z "$GATT" ]; then
  return 0
elif [ -z "$GATT_RAW" ]; then
  return 0
fi

# ============
# 書き出し
# ============
DATE=$(date +%Y-%m-%d)
TIME=$(date +%H:%M:%S)
echo "${DATE}T${TIME}+09:00,${GATT}" >> /home/sierra/csv/env4.csv

