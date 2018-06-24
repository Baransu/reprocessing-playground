open Reprocessing;

type runningT =
  | Loading
  | Running;

type bulletT = {
  pos: (float, float),
  dir: (float, float),
};

type cameraT = {
  x: float,
  y: float,
};

type stateT = {
  pos: (float, float),
  camera: cameraT,
  running: runningT,
  image: imageT,
  boxes: list((float, float)),
  bullets: list(bulletT),
};

/* Load assets */
/* returns initial state  */
let setup = env => {
  Env.resizeable(true, env);
  Env.size(~width=700, ~height=700, env);
  {
    pos: (0., 0.),
    camera: {
      x: 350.,
      y: 350.,
    },
    running: Loading,
    image: Draw.loadImage(~filename="/assets/sprite.png", ~isPixel=true, env),
    boxes: [(300.0, 350.0)],
    bullets: [],
  };
};

let drawCircle = (~center, ~radius, env) =>
  Draw.arcf(
    ~center,
    ~radx=radius,
    ~rady=radius,
    ~start=0.0,
    ~stop=Constants.two_pi,
    ~isPie=false,
    ~isOpen=false,
    env,
  );

let applyCameraView = ((x, y), state, env) => {
  let width = Env.width(env) |> float_of_int;
  let height = Env.height(env) |> float_of_int;
  let x = x -. state.camera.x +. width /. 2.;
  let y = y -. state.camera.y +. height /. 2.;
  (x, y);
};

let renderCharacter = (state, env) => {
  Draw.fill(Constants.red, env);
  let (x, y) = applyCameraView(state.pos, state, env);
  Draw.pushMatrix(env);
  Draw.translate(~x, ~y, env);
  drawCircle(~center=(0., 0.), ~radius=20.0, env);
  Draw.popMatrix(env);
};

let renderBullet = (bullet: bulletT, state, env) => {
  Draw.fill(Constants.green, env);
  let (x, y) = applyCameraView(bullet.pos, state, env);
  Draw.pushMatrix(env);
  Draw.translate(~x, ~y, env);
  drawCircle(~center=(0., 0.), ~radius=5.0, env);
  Draw.popMatrix(env);
};

let renderBox = (box, state, env) => {
  let (x, y) = applyCameraView(box, state, env);
  Draw.pushMatrix(env);
  Draw.translate(~x, ~y, env);
  Draw.rectf(~pos=(0., 0.), ~width=100.0, ~height=100.0, env);
  Draw.popMatrix(env);
};

let updateBullet = (bullet, env) => {
  let velocity = 320.0 *. Env.deltaTime(env);
  let (dx, dy) = Tuple.map(a => a *. velocity, bullet.dir);
  let (px, py) = bullet.pos;
  {...bullet, pos: (px +. dx, py +. dy)};
};

let cleanupBullets = (env, bullets) =>
  List.filter(
    (bullet: bulletT) => {
      let width = Env.width(env);
      let height = Env.height(env);
      let (x, y) = Tuple.map(int_of_float, bullet.pos);
      x < width && x > 0 && y < height && y > 0;
    },
    bullets,
  );

let getSpeed = env => 150.0 *. Env.deltaTime(env);

let moveLeft = (speed, (x, y)) => (x -. speed, y);
let moveRight = (speed, (x, y)) => (x +. speed, y);
let moveUp = (speed, (x, y)) => (x, y -. speed);
let moveDown = (speed, (x, y)) => (x, y +. speed);

let updatePosition = (pos, env) =>
  List.fold_left(
    (pos, (key, fn)) =>
      switch (Env.key(key, env)) {
      | true => fn(getSpeed(env), pos)
      | _ => pos
      },
    pos,
    [(A, moveLeft), (D, moveRight), (W, moveUp), (S, moveDown)],
  );

/* Update takes state/env and returns next state */
let update = (state, env) => {
  let pos = updatePosition(state.pos, env);
  let bullets =
    (
      if (Env.mousePressed(env)) {
        let pos = Tuple.map(float_of_int, Env.mouse(env));
        let dir = Vector.sub(state.pos, pos) |> Vector.normalize;
        let bullet = {pos: state.pos, dir};
        [bullet, ...state.bullets];
      } else {
        state.bullets;
      }
    )
    |> List.map(bullet => updateBullet(bullet, env))
    |> cleanupBullets(env);

  {...state, pos, bullets};
};

let render = (env, state) => {
  Draw.background(Constants.black, env);
  Draw.fill(Constants.blue, env);

  List.map(box => renderBox(box, state, env), state.boxes) |> ignore;

  renderCharacter(state, env);

  List.map(bullet => renderBullet(bullet, state, env), state.bullets)
  |> ignore;

  state;
};

let draw = (state, env) => update(state, env) |> render(env);

let start = () => run(~setup, ~draw, ());