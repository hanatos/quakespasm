# quake as a service

this is a cannibalised version of [quakespasm](https://github.com/sezero/quakespasm.git).
it only compiles a static library to run the quake game mechanics in your own engine.
in particular i've removed all opengl/sdl parts of the code.
currently you'll need to implement `void IN_Move(usercmd_t *cmd)` as well
as `void QS_texture_load(gltexture_t *glt, uint32_t *data)` to compile.
