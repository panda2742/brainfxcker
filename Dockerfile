FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY src/ ./src/
COPY inc/ ./inc/
COPY Makefile ./
RUN make

FROM ubuntu:22.04 AS runtime

RUN apt-get update && apt-get install -y \
    libssl3 \
    libcurl4 \
    valgrind \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY programs/ ./programs/
COPY --from=builder /app/brainfxcker .

ENV VGFLAGS=--leak-check=full --show-leak-kinds=all --track-origins=yes --show-mismatched-frees=yes --track-fds=yes --trace-children=yes

RUN useradd -m appuser
USER appuser

CMD ["./brainfxcker", "programs/main.bf"]
