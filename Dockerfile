FROM ubuntu:latest
WORKDIR /src
RUN apt-get update -y && \
    apt-get install -y g++

# Copy the current directory contents into the container
COPY . .

# Compile the program
RUN g++ -o main src/main.cpp

CMD ["./main"]