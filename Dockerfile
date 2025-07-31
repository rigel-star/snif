FROM python:3.11-slim as builder

WORKDIR /build
COPY src/ ./src

RUN apt-get update && apt-get install -y --no-install-recommends \
    gcc libpcap-dev && \
    gcc -shared -fPIC ./src/library.c ./src/sniffer.c -lpcap -o libsnif.so

FROM python:3.11-slim

RUN apt-get update && apt-get install -y --no-install-recommends libpcap0.8 curl && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY ./*.py .
COPY requirements.txt ./
COPY ./static ./static
COPY ./templates ./templates
COPY --from=builder /build/libsnif.so .

RUN pip install --no-cache-dir -r requirements.txt

ENV FLASK_APP=app.py
ENV FLASK_RUN_HOST=0.0.0.0
ENV FLASK_RUN_PORT=9000
ENV FLASK_DEBUG=1

EXPOSE 9000

CMD ["flask", "run", "--with-threads"]
