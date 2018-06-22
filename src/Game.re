open Reprocessing;

type runningT =
  | Loading
  | Running;

type stateT = {
  pos: (float, float),
  running: runningT,
  image: imageT,
};

/* Load assets */
/* returns initial state  */
let setup = env => {
  Env.resizeable(true, env);
  Env.size(~width=800, ~height=800, env);
  {
    pos: (0.0, 0.0),
    running: Loading,
    image: Draw.loadImage(~filename="/assets/sprite.png", ~isPixel=true, env),
  };
};

let drawImage = (state, env) => {
  let pos = Tuple.map(int_of_float, state.pos);
  Draw.subImage(
    state.image,
    /* on canvas size */
    ~pos,
    ~width=10,
    ~height=10,
    /* position in spritesheet */
    ~texPos=(0, 0),
    ~texWidth=100,
    ~texHeight=100,
    env,
  );
};

let move = (env, a) => a +. 100.0 *. Env.deltaTime(env);

/* Update takes state/env and returns next state */
let update = (state, env) => {
  /* Update state on every frame here */
  let next = {...state, pos: Tuple.map(move(env), state.pos)};
  /* Js.log(state); */
  next;
};

let render = (state, env) => {
  let pos = Tuple.map(int_of_float, state.pos);
  Draw.background(Utils.color(~r=199, ~g=217, ~b=229, ~a=255), env);
  Draw.fill(Utils.color(~r=41, ~g=166, ~b=244, ~a=255), env);
  drawImage(state, env);
  ();
};

let draw = (state, env) => {
  let next = update(state, env);
  render(next, env);
  next;
};

let start = () => run(~setup, ~draw, ());