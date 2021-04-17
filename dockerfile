FROM centos:7
MAINTAINER yeejiac
RUN yum -y update
RUN yum -y install gcc-c++
RUN g++ -v
RUN yum install mysql-devel -y
COPY . .
# RUN ./trader.out