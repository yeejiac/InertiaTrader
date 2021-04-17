FROM centos:7
MAINTAINER yeejiac
RUN yum -y update
RUN yum -y install gcc-c++
RUN g++ -v
RUN yum install mysql-devel -y
RUN export LD_LIBBRARY_PATH="/usr/lib64"
RUN yum install make -y
COPY . .
RUN make main
# RUN ./trader.out