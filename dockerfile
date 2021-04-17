FROM centos:7
MAINTAINER yeejiac
RUN yum -y update
RUN yum -y install gcc-c++
RUN g++ -v
RUN yum install mysql-devel -y
RUN yum install make -y
RUN env CPPFLAGS='-I/usr/lib64/' LDFLAGS='-L/usr/lib64/'
COPY . .
RUN make main
# RUN ./trader.out