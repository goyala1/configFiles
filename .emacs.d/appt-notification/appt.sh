#!/bin/sh

TIME="$1"
MSG="$2"

notify-send -t 0 "<br>Appointment in $TIME minutes:<br>$MSG<br>"
