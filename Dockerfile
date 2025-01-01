FROM almalinux:9

COPY build/pihole-influx-exporter /usr/local/bin

RUN chmod +x /usr/local/bin/pihole-influx-exporter

CMD [ "/usr/local/bin/pihole-influx-exporter" ]
