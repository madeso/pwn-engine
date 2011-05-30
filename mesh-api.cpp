ResourceLoader res;
MeshDef def = res.load("def");
MeshInstance ins = world.add(def);
Animation anim = load("anim");
ins->setPose(anim.getPose(0.75));