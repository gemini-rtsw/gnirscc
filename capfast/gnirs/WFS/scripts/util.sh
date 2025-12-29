#! /bin/echo Subroutine library - do not execute

# $Id: util.sh,v 1.2 2009/05/27 19:35:08 fkraemer Exp $

#
# This is a collection of subroutines for use by shell scripts.  It 
# assumes that config.par has already been sourced.
#

#
# Define mechanism prefixes.
#

ccApply=${prefix}cc:apply
wfsApply=${prefix}wfs:apply

ccCov="${prefix}cc:cov"
ccFilt1="${prefix}cc:filt1"
ccFilt2="${prefix}cc:filt2"
ccFilt3="${prefix}cc:filt3"
ccFoc="${prefix}cc:foc"
ccFopl="${prefix}cc:fopl"
ccPuvw="${prefix}cc:puvw"
ccSplt="${prefix}cc:splt"
ccSter="${prefix}cc:ster"
wfsFilt="${prefix}wfs:filt"
wfsFoc="${prefix}wfs:foc"
wfsPrb="${prefix}wfs:prb"

engCcCov="${prefix}eng:cc:cov"
engCcFilt1="${prefix}eng:cc:filt1"
engCcFilt2="${prefix}eng:cc:filt2"
engCcFilt3="${prefix}eng:cc:filt3"
engCcFoc="${prefix}eng:cc:foc"
engCcFopl="${prefix}eng:cc:fopl"
engCcPuvw="${prefix}eng:cc:puvw"
engCcSplt="${prefix}eng:cc:splt"
engCcSter1="${prefix}eng:cc:ster1"
engCcSter2="${prefix}eng:cc:ster2"
engWfsFilt="${prefix}eng:wfs:filt"
engWfsFoc="${prefix}eng:wfs:foc"
engWfsPrbx="${prefix}eng:wfs:prbx"
engWfsPrby="${prefix}eng:wfs:prby"

coolLow="${prefix}eng:tmpCool.LOSP"
coolRate="${prefix}eng:tmpCool.RATE"
coolDone="${prefix}eng:tmpMotor1.DMOV"

#
# Define detector commands
#

observe="${dcprefix}dc:observe"
observeCar="${dcprefix}dc:observeC"
imName=${dcprefix}dc:imName
imNum=${dcprefix}dc:imNum
integTime=${dcprefix}dc:integTime.VAL
obsSetup=${dcprefix}dc:obsSetup
obsSetupCar=${dcprefix}dc:obsSetupC

#
# Define CAD commands
#

mark=0
clear=1
preset=2
start=3
stop=4

#
# Define the path to the epics distribution
#

extensions=${epicsdir}/extensions/bin/${hostarch}/

#
# Define some external programs, so that they will work regardless of
# the path and shell.
#

echo=/usr/ucb/echo
caput=${extensions}caput
caget=${extensions}caget

#
# Read a channel and print its value (not to be confused with caget)
#
# Syntax:
#
#     caGet CHANNEL
#
# Global variables: status val
#

caGet () {
	status=0

	if [ $# -ne 1 ]; then
		1>&2 $echo caGet: caGet
		exit 1
	fi
	channel=$1

	val=`$caget $1`
	status=$?
	if [ $status -ne 0 ]; then
		# One retry

		val=`$caget $1`
		status=$?

		if [ $status -ne 0 ]; then
			1>&2 $echo "Unable to read $1"
		fi
	fi

	if [ $status -eq 0 ]; then
		echo $val | awk '{
			printf "%s", $2
			for (i = 3; i <= NF; i++)
				printf " %s", $i
		}'
	fi

	return $status
}

#
# Display the contents of an epics record.  The record must support the
# standard VAL, EGU, and PREC fields.
#
# Syntax:
#
#     recordShowDouble REC1 [REC2 [...]]
#
#         REC1  = EPICS record 1
#         REC2  = EPICS record 2
#
# Global variables: status val
#

recordShowDouble () {
	status=0
	count=0

	if [ $# -eq 0 ]; then
		1>&2 $echo "recordShow: recordShow REC1 [REC2 [...]]"
		status=1
	fi

	while [ $# -ge 1 -a $status -eq 0 ]; do
		if [ $status -eq 0 ]; then
			val="`caGet ${1}.VAL`"
			status=$?
		fi

		if [ $status -eq 0 ]; then
			units="`caGet ${1}.EGU`"
			status=$?
		fi

		if [ $status -eq 0 ]; then
			prec="`caGet ${1}.PREC`"
			status=$?
		fi

		if [ $status -eq 0 ]; then
			printf "$1 = %.${prec}f %s\n" $val $units 
			count=`expr $count + 1`
		fi

		shift
	done

	return $status
}

#
# Display the contents of an epics record.
#
# Syntax:
#
#     recordShow REC1 [REC2 [...]]
#
#         REC1  = EPICS record 1
#         REC2  = EPICS record 2
#
# Global variables: status val count
#

recordShow () {
	status=0
	count=0

	if [ $# -eq 0 ]; then
		1>&2 $echo "recordShow: recordShow REC1 [REC2 [...]]"
		status=1
	fi

	while [ $# -ge 1 -a $status -eq 0 ]; do
		if [ $status -eq 0 ]; then
			val="`caGet ${1}.VAL`"
			status=$?
		fi

		if [ $status -eq 0 ]; then
			printf "$1 = %s\n" $val
			count=`expr $count + 1`
		fi

		shift
	done

	return $status
}

#
# Set a CAD record
#
# Syntax:
#
#     cadSet CAD [A [B]]
#
# Global variables: status
#

cadSet () {
	status=0

	if [ $# -lt 1 -o $# -gt 3 ]; then
		1>&2 echo "cadStart: cadStart CAD [A [B]]"
		status=1
	fi

	cad=$1
	a=::undef::
	b=::undef::
	if [ $# -gt 1 ]; then
		a=$2
	fi
	if [ $# -gt 2 ]; then
		b=$3
	fi

	if [ $status -eq 0 -a "$a" != "::undef::" ]; then
		$caput $cad.A "$a" > /dev/null
		status=$?
		if [ $status -ne 0 ]; then
			1>&2 echo "Cannot set $cad.A"
		fi
	fi

	if [ $status -eq 0 -a "$b" != "::undef::" ]; then
		$caput $cad.B "$b" > /dev/null
		status=$?
		if [ $status -ne 0 ]; then
			1>&2 echo "Cannot set $cad.B"
		fi
	fi

	return $status
}

#
# Mark a CAD record.
#
# Syntax:
#
#     cadMark CAD
#
# Global variables: status
#

cadMark () {
	status=0

	if [ $# -ne 1 ]; then
		1>&2 echo "cadMark: cadMark CAD"
		status=1
	fi
	cad=$1

	if [ $status -eq 0 ]; then 
		$caput $cad.DIR $mark > /dev/null
		status=$?
		if [ $status -ne 0 ]; then
			1>&2 echo "Cannot mark $cad"
		fi
	fi

	return $status
}

#
# Preset a CAD record.
#
# Syntax:
#
#     cadPreset CAD
#
# Global variables: status val
#

cadPreset () {
	status=0

	if [ $# -ne 1 ]; then
		1>&2 echo "cadPreset: cadPreset CAD"
		status=1
	fi
	cad=$1

	if [ $status -eq 0 ]; then 
		$caput $cad.DIR $mark > /dev/null
		status=$?
		if [ $status -ne 0 ]; then
			1>&2 echo "Cannot mark $cad"
		fi
	fi

	if [ $status -eq 0 ]; then
		val="`caGet $cad`"
		status=$?
	fi

	if [ $status -eq 0 -a $val -eq -1 ]; then
		val="`caGet $cad.MESS`"
		status=$?

		if [ $status -eq 0 ]; then
			1>&2 echo Configuration Rejected: "$val"
		else
			1>&2 echo Configuration Rejected
		fi

		status=1
	fi

	return $status
}

#
# Start an operation via a CAD record.
#
# Syntax:
#
#     cadStart CAD
#
# Global variables: status val
#

cadStart () {
	status=0

	if [ $# -ne 1 ]; then
		1>&2 echo "cadStart: cadStart CAD"
		status=1
	fi
	cad=$1

	if [ $status -eq 0 ]; then 
		$caput $cad.DIR $start > /dev/null
		status=$?
		if [ $status -ne 0 ]; then
			1>&2 echo "Cannot start $cad"
		fi
	fi

	if [ $status -eq 0 ]; then
		val="`caGet $cad`"
		status=$?
	fi

	if [ $status -eq 0 -a $val -eq -1 ]; then
		val="`caGet $cad.MESS`"
		status=$?

		if [ $status -eq 0 ]; then
			1>&2 echo Start Rejected: "$val"
		else
			1>&2 echo Start Rejected
		fi

		status=1
	fi

	return $status
}

#
# Wait for an operation to complete by monitoring the appropriate
# CAR record(s).  Call a callback function to provide feedback about
# the current position.
#
# Syntax:
#
#     carCWait CALLBACK CAR1 [CAR2 [...]]
#
# Global variables: status
#

carCWait () {
	status=0

	if [ $# -lt 2 ]; then
		1>&2 echo "carCWait: carCWait CALLBACK CAR1 [CAR2 [...]]"
		status=1
	fi

	callback=$1
	shift

	args=$@

	busy=1
	while [ $status -eq 0 -a "$busy" -eq 1 ]; do 
		busy=0
		for car in $args; do
			if [ $status -eq 0 ]; then
				test="`caGet $car.VAL`"
				status=$?

				if [ $status -eq 0 -a "$test" = "BUSY" ]; then
					busy=1
				fi
			fi
		done

		if [ $status -eq 0 -a $busy -eq 1 -a "$callback" != "" ]; then
			$callback
			status=$?
		fi

		sleep 1
	done

	return $status
}

#
# Check the status of the listed CAR record(s).  Print error messages
# if appropriate.
#
# Syntax:
#
#     carShow CAR1 [CAR2 [...]]
#
# Global variables: status
#

carShow () {
	status=0

	if [ $# -lt 1 ]; then
		1>&2 echo "carShow: carShow CALLBACK CAR1 [CAR2 [...]]"
		status=1
	fi

	args=$@

	err=0
	for car in $args; do
		if [ $status -eq 0 ]; then
			test="`caGet $car.VAL`"
			status=$?

			if [ $status -eq 0 -a "$test" = "ERR" ]; then
				mess="`caGet $car.OMSS`"

				if [ $? -eq 0 ]; then
					1>&2 echo "$car: Error $mess"
				else 
					1>&2 echo "$car: Error"
				fi
				err=1
			fi
		fi
	done

	if [ $err -ne 0 ]; then
		status=1
	fi

	return $status
}

#
# Wait for an operation to complete by monitoring the appropriate
# CAR record(s).
#
# Syntax:
#
#     carWait CAR1 [CAR2 [...]]
#

carWait () {
	status=0

	if [ $# -lt 1 ]; then
		1>&2 echo "carWait: carWait CAR1 [CAR2 [...]]"
		status=1
	fi

	if [ $status -eq 0 ]; then
		carCWait "" $@
	fi
}

#
# Clear an apply record.
#

applyClear () {
	status=0

	if [ $# -lt 1 ]; then
		1>&2 echo "applyClear: applyClear CAR1 [CAR2 [...]]"
		status=1
	fi

	while [ $# -gt 0 ]; do
		apply=$1

		$caput $apply.DIR $clear > /dev/null

		shift
	done

	return $status
}

#
# Preset an apply record.
#
# Syntax: applyPreset
#
# Global variables: val
#

applyPreset () {
	status=0

	if [ $# -lt 1 ]; then
		1>&2 echo "applyPreset: applyPreset CAR1 [CAR2 [...]]"
		status=1
	fi

	while [ $# -gt 0 -a $status -eq 0 ]; do
		apply=$1

		$caput $apply.DIR $preset > /dev/null
		status=$?

		if [ $status -eq 0 ]; then
			val="`caGet $apply`"
			status=$?
		fi

		if [ $status -eq 0 -a $val -eq -1 ]; then
			1>&2 echo Configuration Rejected
			status=1
		fi

		shift
	done

	return $status
}

#
# Start an apply record.
#

applyStart () {
	status=0

	if [ $# -lt 1 ]; then
		1>&2 echo "applyStart: applyStart CAR1 [CAR2 [...]]"
		status=1
	fi

	while [ $# -gt 0 ]; do
		apply=$1

		$caput $apply.DIR $start > /dev/null
		status=$?

		shift
	done

	return $status
}

setCoolRate () {
	if [ $# -ne 1 ]; then
		1>&2 echo "setRate: setRate RATE"
		status=1
	fi
	rate=$1

	if [ $status -eq 0 ]; then
		$caput $coolLow $rate > /dev/null
		status=$?
	fi

	if [ $status -eq 0 ]; then
		done=`caGet $coolDone`
		status=$?
	fi

	if [ $status -eq 0 -a "$done" -eq 0 ]; then
		$caput $coolRate 0 > /dev/null
		status=$?

		while [ $status -eq 0 -a $done -eq 0 ]; do
			sleep 1
			done=`caGet $coolDone`
			status=$?
		done
	fi

	if [ $status -eq 0 ]; then
		$caput $coolRate 1 > /dev/null
		status=$?
	fi

	done=1
	while [ $status -eq 0 -a $done -eq 1 ]; do 
		done=`caGet $coolDone`
		status=$?
	done

	return $status
}
