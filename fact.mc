function main() {
  printf("%d\n", fact(4));
  return 0;
}

function fact(n) {
  if (n == 0) {
    return 1;
  } else {
    return n * fact(n-1);
  }
}