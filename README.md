**Мельник Денис 238**

ids:
321152881
321153141
321152865
321152857


classic_merge
Average time:           1.292 ms (theory: omega(nlogn))
Average comparison:     15817.461

classic_quick
Average time:           16.443 ms (theory: omega(nlogn))
Average comparison:     1078462.358

msd_radix
Average time:           0.910 ms (theory: omega(n))
Average comparison:     0.000

msd_radix_quick
Average time:           0.590 ms (theory: omega(n))
Average comparison:     20176.364

string_merge
Average time:           1.255 ms (theory: omega(nlogn))
Average comparison:     21115.302

string_quick
Average time:           0.704 ms (theory: omega(nlogn))
Average comparison:     53826.807


Я прогнал тесты каждого алгоритма на 10 массивах, на каждом из них брал срезы от 100 до 3000 с шагом 100. По этим данным мы действительно видим, что msd_radix_quick работает быстрее msd_radix засчёт условия. А стандартный QuickSort самый медленный. В cpp файле можно видеть, как получаются эти результаты, в py файле только построение графиков.



![1](1.png)
![2](2.png)
