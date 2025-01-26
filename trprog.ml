program
var 
type r : record
    x, y : int,
    c, d : bool
end
a, bc, def: int, 
g, hi, jkl: bool;

begin
r.x := 5;
a := 10 + 2;
bc := 10 + r.x;
def := a + bc;

g := true;
hi := false;
r.d := g and hi or g and not hi;

write(r.x);
write(a);
write(bc);
write(def);
write(g);
write(hi);
write(r.d);

end
@