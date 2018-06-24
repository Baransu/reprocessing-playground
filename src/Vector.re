let sub = ((ax, ay), (bx, by)) => (bx -. ax, by -. ay);

let normalize = ((x, y)) => {
  let length = sqrt(x *. x +. y *. y);
  (x /. length, y /. length);
};