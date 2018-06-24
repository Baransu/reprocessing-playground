let map = (fn, (x, y)) => (fn(x), fn(y));

let mapFirst = (fn, (x, y)) => (fn(x), y);

let mapSecond = (fn, (x, y)) => (x, fn(y));