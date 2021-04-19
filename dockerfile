FROM centos:7
MAINTAINER yeejiac
RUN yum -y update
RUN yum -y install gcc-c++
RUN g++ -v
RUN yum install mysql-devel -y
RUN yum install make -y
RUN ln -s /usr/lib64/libmysqlclient.so /usr/bin/libmysqlclient.so
COPY . .
RUN make main
# RUN ./trader.out