#!/usr/bin/env python2.6

import pylab, os
from subprocess import call

filelist = ['en_vs_rho_k0.dat', 'pidle_vs_rho_k0.dat', 'ploss_vs_rho_k0.dat', 'en_vs_rho_k5.dat', 'pidle_vs_rho_k5.dat', 'ploss_vs_rho_k5.dat', 'en_vs_rho_k10.dat', 'pidle_vs_rho_k10.dat', 'ploss_vs_rho_k10.dat', 'en_vs_rho_k40.dat', 'pidle_vs_rho_k40.dat', 'ploss_vs_rho_k40.dat']  

figure = 0

pylab.rc('font', **{'family':'sans-serif','sans-serif':['Helvetica']})

os.chdir(os.path.dirname(os.path.realpath(__file__)) + "/..")
call(["make", "distclean"])
call(["make"])
call(["./sim", "-t 10000", "-c 1000000", "-l 12000", "-r 0.25,0.95"])

pylab.figure(++figure)
pylab.clf()
data, label = pylab.loadtxt(filelist[0],delimiter=','), r'K=$\infty$'
pylab.plot(data[:,0], data[:,1],label=label)
pylab.title(r"$E[N]$ vs $\rho$")
pylab.xlabel(r"Utilization of the buffer")
pylab.ylabel(r"Average \# of packets in the buffer")
pylab.legend()
pylab.savefig(filelist[0].rsplit('.',1)[0]+".png")

pylab.figure(++figure)
pylab.clf()
data, label = pylab.loadtxt(filelist[0],delimiter=','), r'K=$\infty$'
pylab.plot(data[:,0], data[:,1], label=label)
pylab.title(r"$P_{idle}$ vs $\rho$")
pylab.xlabel(r"Utilization of the buffer")
pylab.ylabel(r"Proportion of time the system is idle")
pylab.legend()
pylab.savefig(filelist[1].rsplit('.',1)[0]+".png")

call(["./sim", "-t 10000", "-c 1000000", "-l 12000", "-r 0.5,1.5", "-k 5"])
call(["./sim", "-t 10000", "-c 1000000", "-l 12000", "-r 0.5,1.5", "-k 10"])
call(["./sim", "-t 10000", "-c 1000000", "-l 12000", "-r 0.5,1.5", "-k 40"])

pylab.figure(++figure)
pylab.clf()
data, label = pylab.loadtxt(filelist[3],delimiter=','), 'K=5'
pylab.plot(data[:,0], data[:,1], label=label)
data, label = pylab.loadtxt(filelist[6],delimiter=','), 'K=10'
pylab.plot(data[:,0], data[:,1], label=label)
data, label = pylab.loadtxt(filelist[9],delimiter=','), 'K=40'
pylab.plot(data[:,0], data[:,1], label=label)
pylab.title(r"$E[N]$ vs $\rho$")
pylab.xlabel(r"Utilization of the buffer")
pylab.ylabel(r"Average \# of packets in the buffer")
pylab.legend()
pylab.savefig("en_vs_rho_k_finite.png")

call(["./sim", "-t 10000", "-c 1000000", "-l 12000", "-r 0.4,10", "-k 5"])
call(["./sim", "-t 10000", "-c 1000000", "-l 12000", "-r 0.4,10", "-k 10"])
call(["./sim", "-t 10000", "-c 1000000", "-l 12000", "-r 0.4,10", "-k 40"])

pylab.figure(++figure)
pylab.clf()
data, label = pylab.loadtxt(filelist[5],delimiter=','), 'K=5'
pylab.plot(data[:,0], data[:,1], label=label)
data, label = pylab.loadtxt(filelist[8],delimiter=','), 'K=10'
pylab.plot(data[:,0], data[:,1], label=label)
data, label = pylab.loadtxt(filelist[11],delimiter=','), 'K=40'
pylab.plot(data[:,0], data[:,1], label=label)
pylab.title(r"$P_{loss}$ vs $\rho$")
pylab.xlabel(r"Utilization of the buffer")
pylab.ylabel(r"Packet loss ratio")
pylab.legend()
pylab.savefig("ploss_vs_rho_k_finite.png")

