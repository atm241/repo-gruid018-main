# Assignment 5 (Art Render) Worksheet

## Q1: Phong Shading

Given the following lighting equation:

*diffuse* = *n* &middot; *l*

*specular* = (*n* &middot; *h*) ^ *s*

*color* = ka\*Ia + kd\*Id\**diffuse* + ks\*Is\**specular*

Draw a picture that includes a point on a surface, a light, and labeled arrows
for each vector that shows up in the equation. Hint: make sure that your
vectors point in the right direction to make sense for the equation as written
(e.g., make sure you draw *l* pointing in the correct direction for *n*
&middot; *l* to be calculated correctly)!

Replace this image with your diagram:

![](./img/vectors.png)


## Q2: Silhouette Outline

This week in class we'll be talking in more detail about the key matrices used
in vertex and fragment shaders. For example, we'll learn that the
`normal_matrix` must be used rather than the `model_view_matrix` to transform
normals to eye (a.k.a. camera) space. You'll use this in all of the shaders
you write. The outline shader includes the most interesting use of normals
though because not only does each vertex have a normal, the shader also has
access to the "left normal" for the normal of the triangle to the left and the
"right normal" for the triangle to the right. As you see in the assignment
handout these are used to determine whether the vertex lies on a silhouette
edge. Here are a few questions about the logic you'll need to use in that
shader:

### Q2.1
Your outline vertex shader will need to include an if statement that is true
if the vertex lies on a silhouette edge by testing the left normal and right
normal in some way. Assuming `vec3 e` is a vector calculated in eye space
that points from the vertex to the eye and `vec3 nl` is defined for the left
normal and `vec3 nr` for the right normal, fill in the condition on the if
statement:

```
if (((nr.Dot(e)) * (nl.Dot(e)) < 0)
```

### Q2.2
For the `nl` and `nr` that appear in your if statement above, should these two
vectors be transformed to eye space using the `normal_matrix`?

```
/* yes assuming that e was calculated in eye space as stated in the previous prompt */
```

### Q2.3
Inside the "if statement" from Q2.1, you will need to offset the vertex in the
direction of the normal to move it outwards in order to create the "fin" that
forms the silhouette outline. This process of changing the location of the vertex is like making a change to the actual 3D geometry of the model, as if you quickly loaded the model into a 3D modeling program, edited the vertex by hand, and resaved the file.  So, we want to make this change while the vertex is still in model space, before transforming it to world space, camera space, and so on.  With this in mind, which version of the vertex normal should you use at this step?  Should you transform the normal as usual by multiplying by the `normal_matrix` in this case?

```
/* no since we want the outline in object space and multiplying by the normal space gives us eye space */
```
