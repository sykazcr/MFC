using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DrawGeoMetry : MaskableGraphic
{
    List<UIVertex> vertexList = new List<UIVertex>();
    [Range(0, 360)]
    public float fillRange;
    public Texture texture;
    public float radio;
    [Range(3, 360)]
    public int segment;
    public float lineWidth = 10;

    public override Texture mainTexture {
        get
        {
            return texture;
        }
    }
    protected override void OnPopulateMesh(VertexHelper vh)
    {
        //vh.Clear();

        //if (vertexList.Count > 0)
        //{
        //    for (int i = 0; i < vertexList.Count; i++)
        //    {
        //        UIVertex[] uIVertices = new UIVertex[4];
        //        UIVertex uVertex;

        //        vh.AddUIVertexQuad(uIVertices);
        //    }
        //}
    }
}
