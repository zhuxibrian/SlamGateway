#!/bin/bash
DIR=`cd "$(dirname "$0")" ; pwd`
cd "$DIR"

SERVER=localhost
PORT=61883
USER=user
PASSWD=PASSWORDus
# man mqtt
# ·   0: The broker/client will deliver the message once, with no confirmation.
# ·   1: The broker/client will deliver the message at least once, with confirmation required.
# ·   2: The broker/client will deliver the message exactly once by using a four step handshake.
MQTT_QOS=2

mqtt_sub(){
	local topic=$1
	mosquitto_sub -h $SERVER -p $PORT -P $PASSWD -q $MQTT_QOS -t $topic -u $USER -v
}
mqtt_pub(){
	local topic=$1
	local msg=$2
	mosquitto_pub -h $SERVER -p $PORT -P $PASSWD -q $MQTT_QOS -u $USER -t $topic -m $msg
}

case $1 in
start-server)
	if [ "$SERVER"x = "localhost"x ]; then
		killall mosquitto
		echo \
'port '"$PORT"'
listener 9005
protocol websockets
allow_anonymous false
password_file '"$DIR/mosquitto_pwfile" \
		>$DIR/mosquitto.conf
		touch $DIR/mosquitto_pwfile
		mosquitto_passwd -b $DIR/mosquitto_pwfile $USER $PASSWD
		mosquitto -d -c $DIR/mosquitto.conf 1>/dev/null 2>&1
	fi
	mosquitto_sub -h $SERVER -p $PORT -P $PASSWD -q $MQTT_QOS -t "#" -u $USER -v
	echo "subscribe start"
	echo "port:$PORT user:$USER password:$PASSWD"
	;;	
sub)
	mqtt_sub $2
	;;
pub)
	mqtt_pub $2 $3
	;;
emu-ctrl)
	mqtt_sub '/rw/ctrl/all' |
	while true 
	do 
		read LINE
		echo $LINE
		l=($LINE)
		if [ "${l[0]}"x = "/rw/ctrl/all"x ]; then
			echo '
			{
				"messageType":"info",
				"timestamp":'`date +%s`',
				"ip":"127.0.0.1",
				"state":"idle",
				"battery":99,
				"serveState":"down",
				"faultInfo":""
			}
			' | mosquitto_pub -h $SERVER -p $PORT -P $PASSWD -q $MQTT_QOS -u $USER -t '/rw/ctrl/0' -s 
			
			echo '
			{
				"messageType":"info",
				"timestamp":'`date +%s`',
				"ip":"127.0.0.1",
				"state":"idle",
				"battery":99,
				"serveState":"down",
				"faultInfo":""
			}
			' | mosquitto_pub -h $SERVER -p $PORT -P $PASSWD -q $MQTT_QOS -u $USER -t '/rw/ctrl/0' -s 
		fi
	done
	;;
emu-sch)
	;;
*)
	echo 'usage: test.sh start-server | pub <T> <M> | sub <T> | emu_ctrl |emu_sch '
	;;
esac
