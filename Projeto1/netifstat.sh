#!/bin/bash

###############################################################################################################################
#                                               SO - Trabalho prático 1
#
#   Realizado por:
#       Ana Raquel Paradinha - NMec: 102491
#       Paulo Pinto - NMec: 103234
#
###############################################################################################################################

declare -A rx           # Array associativo que guarda os valores de rx
declare -A tx           # Array associativo que guarda os valores de tx        # O index corresponde ao nome da interface
declare -A rrate        # Array associativo que guarda os valores de rrate
declare -A trate        # Array associativo que guarda os valores de trate     
declare -A rx_last      # Array associativo que guarda os valores de rx do loop anterior
declare -A tx_last      # Array associativo que guarda os valores de tx do loop anterior
declare -A rx_total     # Array associativo que guarda a soma dos valores de rx a cada loop
declare -A tx_total     # Array associativo que guarda a soma dos valores de tx a cada loop

rexp='^[0-9]+(\.[0-9]*)?$'      # Expressão regex para verificar se o último arg é um número
SEC=${@: -1}                    # Último argumento é o número de segundos
NAME=""                         # Expressão regex para selecionar as interfaces
NUMBER=""                       # Número de interfaces que queremos ver
ctrl=0                          # Valor de controlo para as opções de unidades
ord=0                           # Valor de controlo para as opções de ordenação
exp=0                           # Valor do expoente para fazer a conversão de unidades
loop=0                          # Ativa ou desativa o loop
col=1                           # Identifica a coluna por onde vão ser ordenados os dados
reverse=""                      # Ativa ou desativa o sort reverso
turn=0                          # Indica se é a primeira vez que o loop é executado


# Lista as opções disponíveis 
options() {
    echo "-----------------------------------------------------------------------------------"
    echo "${@:0} -c [NAME] -b|-k|-m -p [NUMBER] -t|-r|-T|-R -v -l [SEC]"
    echo
    echo "OPÇÕES DISPONÍVEIS!"
    echo
    echo "    -c    : Selecionar as interfaces a analisar através de uma expressão regular"
    echo "    -p    : Defenir o número de interfaces a visualizar"
    echo "    -l    : Analisar as interfaces de s em s segundos"
    echo "    -v    : Fazer um sort reverso"
    echo "O último argumento tem de corresponder sempre ao número de segundos que pretende analisar."
    echo
    echo "Opções de unidades (usar apenas 1):"
    echo "    -b    : Valores em bytes (default)"
    echo "    -k    : Valores em Kilobytes"
    echo "    -m    : Valores em Megabytes"
    echo
    echo "Opções de ordenação (usar apenas 1):"
    echo "    -t    : Ordenar pelo TX"
    echo "    -r    : Ordenar pelo RX"
    echo "    -T    : Ordenar pelo TRATE"
    echo "    -R    : Ordenar pelo RRATE"
    echo "A ordenação default é alfabética e para cada opção é decrescente."
    echo "------------------------------------------------------------------------------------"
}

error_exit () {
    options
    exit 1
}

unit_exit () {
    if [[ $ctrl == 1 ]]; then 
        # Quando há mais que 1 argumento de unidades
        echo "ERRO: não é possivel usar -b, -k e -m ao mesmo tempo!" >&2
        error_exit
    else 
        ctrl=1
    fi
}

sort_exit () {
    reverse="r"
    if [[ $ord == 1 ]]; then
        echo "Não é possivel usar -t,-r,-T e -R ao mesmo tempo!" >&2
        error_exit
    else 
        ord=1
    fi
}

# Verifica que o argumento obrigatório está presente
if [[ $# == 0 ]]; then
    echo "ERRO: deve passar pelo menos um argumento (número de segundos a analisar)." >&2
    error_exit
fi

# Verifica que o último argumento é o número de segundos
if ! [[ $SEC =~ $rexp ]]; then 
    echo "ERRO: o último argumento tem de ser o número de segundos que pretende analisar." >&2
    error_exit
fi

set -- "${@:1:$(($#-1))}"   #retira o último arg (sec) para não ser usado como arg das opções

# Tratamento das opções passadas como argumentos
while getopts ":c:bkmp:trTRvl" option; do    
    case $option in
    c) # Seleção das interfaces a visualizar através de uma expressão regular
        NAME=$OPTARG
        ;;
    b)
        unit_exit     # Unidade = Byte
        ;;
    k)
        unit_exit
        exp=1     # Unidade = KiloByte
        ;;
    m)
        unit_exit
        exp=2     # Unidade = MegaByte
        ;;
    p) # Número de interfaces a visualizar
        NUMBER=$OPTARG
        if [[ NUMBER =~ "^[0-9]+$" ]]; then
            echo "ERRO: o número de interfaces tem de ser um inteiro positivo." >&2
            error_exit
        fi
        ;;
    t)
        sort_exit
        col=2
        ;;
    r)
        sort_exit
        col=3
        ;;
    T)
        sort_exit
        col=4
        ;;
    R)
        sort_exit
        col=5
        ;;
    v) #Ordenação reversa
        if [[ $reverse == "r" ]];  then
            reverse=""
        else
            reverse="r"
        fi
        ;;
    l) # Loop
        loop=1
        ;;
    :) # Argumento obrigatório em falta
        echo "ERRO: argumento em falta na opção -${OPTARG}!" >&2
        error_exit
        ;;
    *) #Passagem de argumentos inválidos
        echo "ERRO: opção inválida!" >&2
        error_exit
        ;;
    esac
done

printData() {
    n=0
    un=$((1024 ** exp))
    for net in /sys/class/net/[[:alnum:]]*; do
        if [[ -r $net/statistics ]]; then
            f="$(basename -- $net)"
            if ! [[ $NAME =~ ""  && $f =~ $NAME ]]; then   
                continue                                  
            fi               

            if [[ $turn == 0 ]]; then
                rx_bytes1=$(cat $net/statistics/rx_bytes | grep -o -E '[0-9]+') # está em bytes
                tx_bytes1=$(cat $net/statistics/tx_bytes | grep -o -E '[0-9]+') # está em bytes
                sleep $SEC
            else
                rx_bytes1=rx_last[$f]
                tx_bytes1=tx_last[$f]
            fi

            rx_bytes2=$(cat $net/statistics/rx_bytes | grep -o -E '[0-9]+') #está em bytes
            tx_bytes2=$(cat $net/statistics/tx_bytes | grep -o -E '[0-9]+') #está em bytes
            
            rx[$f]=$((rx_bytes2 - rx_bytes1))
            tx[$f]=$((tx_bytes2 - tx_bytes1))

            rrate[$f]=$(bc <<< "scale=3;${rx[$f]}/$SEC")
            trate[$f]=$(bc <<< "scale=3;${tx[$f]}/$SEC")

            if [[ $loop == 1 ]]; then
                tx_total[$f]=$((tx_total[$f] + tx[$f]))
                rx_total[$f]=$((rx_total[$f] + rx[$f]))
                rx_last[$f]=$rx_bytes2
                tx_last[$f]=$tx_bytes2
            fi
            n=$((n + 1))
        fi
        if [[ $n == $NUMBER ]]; then  
            break
        fi
    done
    n=0
    for net in /sys/class/net/[[:alnum:]]*; do
        if [[ -r $net/statistics ]]; then
            f="$(basename -- $net)"
            if ! [[ $NAME =~ ""  && $f =~ $NAME ]]; then   
                continue                                  
            fi              
            if [[ $loop == 1 ]]; then
                printf "%-12s %12s %12s %12s %12s %12s %12s\n" "$f" "$(bc <<< "scale=3; ${tx[$f]}/$un")" "$(bc <<< "scale=3; ${rx[$f]}/$un")" "$(bc <<< "scale=3;${trate[$f]}/$un")" "$(bc <<< "scale=3;${rrate[$f]}/$un")" "$(bc <<< "scale=3;${tx_total[$f]}/$un")" "$(bc <<< "scale=3;${rx_total[$f]}/$un")"
            else
                printf "%-12s %12s %12s %12s %12s\n" "$f" "$(bc <<< "scale=3; ${tx[$f]}/$un")" "$(bc <<< "scale=3; ${rx[$f]}/$un")" "$(bc <<< "scale=3;${trate[$f]}/$un")" "$(bc <<< "scale=3;${rrate[$f]}/$un")"
            fi
            n=$((n + 1))
        fi
        if [[ $n == $NUMBER ]]; then
            break
        fi
    done | sort -k$col$reverse
}

if [[ $loop == 1 ]]; then
    while true; do
        printf "%-12s %12s %12s %12s %12s %12s %12s\n" "NETIF" "TX" "RX" "TRATE" "RRATE" "TXTOT" "RXTOT"
        printData
        turn=1
        echo ""
        sleep $SEC
    done
else
    printf "%-12s %12s %12s %12s %12s\n" "NETIF" "TX" "RX" "TRATE" "RRATE"
    printData
fi

exit 0
