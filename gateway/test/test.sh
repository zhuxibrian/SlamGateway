#!/bin/sh
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

case $1 in
sub)
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
pub)
	topic=$2
	msg=$3
	mosquitto_pub -h $SERVER -p $PORT -P $PASSWD -q $MQTT_QOS -u $USER -t $topic -m $msg
	;;
*)
	;;
esac
