x <- as.numeric(sample(1:50, 10, replace = TRUE))

microbenchmark::microbenchmark(
  myrle::myrle(x),
  rle(x),
  check = "equal"
)
