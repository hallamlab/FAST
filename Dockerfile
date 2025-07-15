FROM alpine:3.22.0 AS builder
RUN apk add --no-cache g++ make

COPY . /fast
WORKDIR /fast
RUN sed -i 's/-m64//g' Makefile
RUN make

FROM alpine:3.22.0
COPY --from=builder /fast/fastal /app/fastal
COPY --from=builder /fast/fastdb /app/fastdb
RUN apk add --no-cache gcc libstdc++
ENV PATH="/app:${PATH}"
