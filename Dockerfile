FROM almalinux

COPY build/pihole-influx-exporter /usr/local/bin

RUN dnf install -y cronie && \
  chmod +x /usr/local/bin/pihole-influx-exporter && \
  echo "* * * * * bash -c '/usr/local/bin/pihole-influx-exporter >> /var/log/pihole-influx-exporter.log 2>&1'" >> /etc/cron.d/pihole-exporter.conf && \
  chmod 644 /etc/cron.d/pihole-exporter.conf && \
  crontab /etc/cron.d/pihole-exporter.conf

CMD [ "/usr/sbin/crond", "-f" ]
