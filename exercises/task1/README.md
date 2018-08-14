# Task 1

- Contents: Buildroot e Qemu [[Link]](http://www.inf.pucrs.br/emoreno/undergraduate/CC/labsisop/sem18.2/atividades/1.1/1.1%20-%20buildroot.html)

- Observation 1: My first attempt failed to build *uclibc-1.0.26* but it worked after a ran *make* again.

- Observation 2: Second attempt failed at *host-gcc-final-6.4.0*.
```bash
vagrant suspend

vagrant up --provider=parallels #Required since root default provider is virtualbox
```
- Observation 3: MacOS has a bug? in gems or something. Apparently, downgrading from Vagrant v2.1.2 to v2.1.1 fixes the problem. [[Link]](https://releases.hashicorp.com/vagrant/2.1.1/)



buildroot/output/build/uclibc-1.0.26/ldso/libdl/.libdl.oS.dep
Line 237, 