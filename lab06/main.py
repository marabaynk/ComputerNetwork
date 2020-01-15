import ctypes
import platform
import re
import subprocess


def get_choice() -> int:
    ch = 1
    flag = True
    while flag:
        try:
            ch = int(input('[1-menu]> '))
            flag = False
        except ValueError:
            flag = True
    return ch


def print_menu():
    print('1 - меню')
    print('2 - количество сетевых адресов (IPv4/IPv6)')
    print('3 - список активных сетевых устройств')
    print('4 - количество успешно доставленных пакетов')
    print('5 - количество пакетов, которое не удалось доставить')
    print('6 - количество фрагментированных пакетов')
    print('0 - выход')


def get_os():
    plat = platform.system()
    if plat == 'Windows':
        plat = 0
    elif plat == 'Darwin':
        plat = 1
    else:
        plat = -1
    return plat


def print_count_ip():
    print('IPv4: 2^32  = {ip4}'.format(ip4=pow(2, 32)))
    print('IPv6: 2^128 = {ip6}'.format(ip6=pow(2, 128)))


def eval_command(type_os: int, command: str) -> str:
    encoding = '866' if type_os == 0 else 'utf-8'
    return subprocess.check_output(command, shell=True, encoding=encoding)


def print_count_active(type_os: int):
    print('Список активных сетевых устройств:')
    if type_os == 0:
        print(eval_command(type_os, 'netstat -an | findstr "IP TCP UDP"'))
    elif type_os == 1:
        print(eval_command(type_os, 'netstat -an | grep -E "tcp|udp"'))
    else:
        print('Unknown system')


def print_success_package(type_os: int):
    if type_os == 0 or type_os == 1:
        if type_os == 0:
            s = eval_command(type_os, 'netstat -s | findstr /c:"Доставлено полученных пакетов"')
        else:
            s = eval_command(type_os, 'netstat -s | grep "incoming packets delivered"')
        count = sum([int(num) for num in re.findall(r'(\d+)', s)])
        print('Количество успешно доставленных пакетов: {count}'.format(count=count))
    else:
        print('Unknown system')


def print_failed_package(type_os: int):
    if type_os == 0 or type_os == 1:
        if type_os == 0:
            s = eval_command(type_os, 'netstat -s | findstr /c:"Отброшено выходных пакетов"')
        else:
            s = eval_command(type_os, 'netstat -s | grep -w "outgoing packets dropped"')
        count = sum([int(num) for num in re.findall(r'(\d+)', s)])
        print('Количество пакетов, которое не удалось доставить: {count}'.format(count=count))
    else:
        print('Unknown system')


def print_fragmented_package(type_os: int):
    if type_os == 0 or type_os == 1:
        if type_os == 0:
            s = eval_command(type_os, 'netstat -s | findstr /c:"Создано фрагментов"')
        else:
            s = eval_command(type_os, 'ss -s | grep "FRAG"')
        count = sum([int(num) for num in re.findall(r'(\d+)', s)])
        print('Количество фрагментированных пакетов: {count}'.format(count=count))
    else:
        print('Unknown system')


if __name__ == '__main__':
    os = get_os()  # 0 - Windows; 1 - Linux; -1 - Unknown

    if os == 0:
        print('OS - Windows')
    elif os == 1:
        print('OS - Darwin')
    else:
        print('OS - Unknown')
        exit(-1)

    choice = get_choice()

    while choice != 0:
        if choice == 2:  # количество сетевых адресов (IPv4/IPv6)
            print_count_ip()
        elif choice == 3:  # список активных сетевых устройств
            print_count_active(os)
        elif choice == 4:  # количество успешно доставленных пакетов
            print_success_package(os)
        elif choice == 5:  # количество пакетов, которое не удалось доставить
            print_failed_package(os)
        elif choice == 6:  # количество фрагментированных пакетов
            print_fragmented_package(os)
        else:
            print_menu()

        choice = get_choice()
