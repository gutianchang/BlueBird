import os
import sys


argv_netcard = ""

def check_status():
    print("[*]run check_status")
    if len(sys.argv) != 2:
        print("argv is err")
        sys.exit(-1)

    global argv_netcard
    argv_netcard = sys.argv[1]
    ifconfig_process = os.popen('ifconfig')
    ifconfig_result = ifconfig_process.read()
    ifconfig_process.close()
    if ifconfig_result:
        ret = ifconfig_result.find(argv_netcard)
        if ret > 0:
            print(argv_netcard + " status: up")
            return ret
        else:
            print(argv_netcard + "status: down")
            return ret
    print("ifconfig: " + argv_netcard + "not found")
    return -1

def check_mode_monitor():
    print("[*]run check_mode_monitor")
    str = "iwconfig " + argv_netcard
    iwconfig_process = os.popen(str)
    iwconfig_result = iwconfig_process.read()
    iwconfig_process.close()
    if iwconfig_result:
        ret = iwconfig_result.find("Mode:Monitor")
        if ret > 0:
            print(argv_netcard + " mode: monitor")
            return ret
        else:
            print(argv_netcard + " mode not monitor")
            return ret
    print("iwconfig: " + argv_netcard + " not found")
    return -1

def set_mode_monitor():
    print("[*]run set_mode_monitor")
    cmd_str1 = "ifconfig " + argv_netcard + " down"
    os.system(cmd_str1)
    cmd_str2 = "iwconfig " + argv_netcard + " mode monitor"
    os.system(cmd_str2)
    cmd_str3 = "ifconfig " + argv_netcard + " up"
    os.system(cmd_str3)
    check_mode_monitor()


if __name__ == '__main__':
    ret = check_status()
    if ret > 0:
        ret = check_mode_monitor()
        if ret > 0:
            pass
        else:
            set_mode_monitor()
    else:
        set_mode_monitor()
