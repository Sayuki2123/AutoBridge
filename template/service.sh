#!/system/bin/sh

MODDIR=${0%/*}

data_dir="/data/local/tmp/autobridge"
applist="ndk-apps.txt"
request="$data_dir/redirect.req"

link_path="/system/lib64/arm64"
lib_houdini="/vendor/lib64/arm64"
lib_ndk="/system/lib64/arm64_ndk"

log_debug() {
    log -p d -t "AutoBridge64" "[Service] $1"
}

log_debug "Start at $MODDIR"

if [ ! -e $data_dir ]; then
    mkdir -p $data_dir
fi

if [ ! -e "$data_dir/$applist" ]; then
    cp "$MODDIR/$applist" $data_dir
fi

> $request
chmod +rw $request

while true; do
    if [ -s $request ]; then
        if [[ $(readlink $link_path) == $lib_ndk ]]; then
            ln -sfn $lib_houdini $MODDIR$link_path
            log_debug "Redirect to Houdini"
        else
            ln -sfn $lib_ndk $MODDIR$link_path
            log_debug "Redirect to NDK_translation"
        fi

        mount -o remount /system
        > $request

        sleep 1
    fi

    sleep 1
done
