FROM almalinux

COPY build/pihole-influx-exporter /usr/local/bin

RUN chmod +x /usr/local/bin/pihole-influx-exporter && \
  mkdir /etc/cron.d && \
  echo "* * * * * /usr/local/bin/pihole-influx-exporter >> /var/log/pihole-influx-exporter.log 2>&1" >> /etc/cron.d/pihole-exporter.conf && \
  chmod 644 /etc/cron.d/pihole-exporter.conf

CMD [ "/usr/sbin/crond", "-f" ]
