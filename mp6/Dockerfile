FROM gcc:latest
RUN pip install -r requirements.txt
# # RUN apt-get update
# WORKDIR /mp6 /mp2
COPY /mp2 /mp6
# COPY ./docker/entrypoint.sh /
# RUN chmod +x entrypoint.sh
# ENTRYPOINT ["/entrypoint.sh"]



# FROM gcc:latest
# # RUN apt-get update && apt-get install -y valgrind
# COPY ./docker/entrypoint.sh /
# RUN chmod +x entrypoint.sh
# ENTRYPOINT ["/entrypoint.sh"]