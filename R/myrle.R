#' @export
myrle <- function(x) {

  .Call(my_c_rle, x)

}
