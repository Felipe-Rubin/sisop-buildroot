
# Notes 
O diretorio /proc possui todas as informações necessárias para o projeto.

Toda vez que o usuario acessar a página, o servidor deve atualizar as informacoes.

Da pra usar comandos do próprio linux.
Exemplo: 
- ps : Comandos que estao rodando
- date: Data e hora.

Depois adicionar tudo no github.



## Requirements:
- [] Data e hora do sistema (By default it uses UTC)
```sh
date "+%d/%m/%Y" #Date 21/08/2018
date "+%H:%M:%S" #Time 20:38:50
```
- [] Uptime (tempo de funcionamento sem reinicialização do sistema) em segundos;
```sh
cat /proc/uptime #Two numbers: <system uptime in seconds> <time spent in idle process in seconds>
cat /proc/uptime | awk '{print $1}'
```
- [] Modelo do processador e velocidade;
```sh
lscpu #Displays information about the cpu architecture, information form sysfs and /proc/cpuinfo
cat /proc/cpuinfo #Contains all the information
```
- [] Capacidade ocupada do processador (%);
- [] Quantidade de memória RAM total e usada (MB);
```sh
free -m # -m: Megabytes
```
- [] Versão do sistema;
```sh
uname -a #Prints system information
cat /proc/version #Contains all the information
```
- [] Lista de processos em execução (pid e nome).


Search in man pages
any page containting "this" man -K ""

inside: /search
to go forward: N
backwards: shift+N
```
/proc/stat
      kernel/system statistics.  Varies with architecture.  Common entries include:

      cpu  3357 0 4313 1362393
             The  amount  of  time,  measured in units of USER_HZ (1/100ths of a second on
             most architectures, use sysconf(_SC_CLK_TCK) to obtain the right value), that
             the system spent in various states:

 user   (1) Time spent in user mode.

 nice   (2) Time spent in user mode with low priority (nice).

 system (3) Time spent in system mode.

 idle   (4)  Time  spent in the idle task.  This value should be USER_HZ times
        the second entry in the /proc/uptime pseudo-file.

 iowait (since Linux 2.5.41)
        (5) Time waiting for I/O to complete.

 irq (since Linux 2.6.0-test4)
        (6) Time servicing interrupts.

 softirq (since Linux 2.6.0-test4)
        (7) Time servicing softirqs.

 steal (since Linux 2.6.11)
        (8) Stolen time, which is the time spent in  other  operating  systems
        when running in a virtualized environment

 guest (since Linux 2.6.24)
        (9) Time spent running a virtual CPU for guest operating systems under
        the control of the Linux kernel.

 guest_nice (since Linux 2.6.33)
        (10) Time spent running a niced guest (virtual CPU for guest operating
        systems under the control of the Linux kernel).
```


user nice system idle iowait  irq  softirq steal guest guest_nice

/proc/stat : system + user + iddle = 100%


head -1 /proc/stat


awk '{print $1 + }'



















