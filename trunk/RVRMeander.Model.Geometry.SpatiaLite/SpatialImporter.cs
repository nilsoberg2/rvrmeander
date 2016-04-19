using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MetroFlow.Model.Geometry.SpatiaLite
{
  /// <summary>
  /// This class provides a way for users to convert an object container to an output dataset.
  /// </summary>
  public abstract class SpatialImporter
  {
    /// <summary>
    /// Attempts to import an IObjectContainer into a spatial dataset.
    /// </summary>
    /// <param name="container"></param>
    /// <param name="outputDataset"></param>
    /// <returns></returns>
    public bool TryImport(MetroFlow.Objects.Container.IObjectContainer container, out ISpatialDataset outputDataset)
    {
      outputDataset = null;

      if (container == null)
      {
        return false;
      }

      outputDataset = CreateSpatialDataset();

      //Dictionary<string, ISpatialClass> nodes = new Dictionary<string, ISpatialClass>();

      foreach (var node in container.GetNodes())
      {
        string className = node.Type.ToString();
        ISpatialClass theClass = null;
        if (!outputDataset.ContainsClass(className))
        {
          theClass = CreateSpatialClass(node.Kind, node.GetAttributes());
          theClass.Name = className;
          outputDataset.AddSpatialClass(theClass);
        }
        else
        {
          if (!outputDataset.TryGetSpatialClass(className, out theClass))
          {
            continue;
          }
        }
        theClass.AddSpatialObject(CreateSpatialObject(node));
      }

      foreach (var link in container.GetLinks())
      {
        string className = link.Type.ToString();
        ISpatialClass theClass = null;
        if (!outputDataset.ContainsClass(className))
        {
          theClass = CreateSpatialClass(link.Kind, link.GetAttributes());
          theClass.Name = className;
          outputDataset.AddSpatialClass(theClass);
        }
        else
        {
          if (!outputDataset.TryGetSpatialClass(className, out theClass))
          {
            continue;
          }
        }
        theClass.AddSpatialObject(CreateSpatialObject(link));
      }

      foreach (var sub in container.GetSubcatchments())
      {
        string className = sub.Type.ToString();
        ISpatialClass theClass = null;
        if (!outputDataset.ContainsClass(className))
        {
          theClass = CreateSpatialClass(sub.Kind, sub.GetAttributes());
          theClass.Name = className;
          outputDataset.AddSpatialClass(theClass);
        }
        else
        {
          if (!outputDataset.TryGetSpatialClass(className, out theClass))
          {
            continue;
          }
        }
        theClass.AddSpatialObject(CreateSpatialObject(sub));
      }

      //container.GetObjectsByType<Objects.Nodes.Junction>(Objects.ObjectType.Junction);

      return true;
    }

    public abstract ISpatialObject CreateSpatialObject(Objects.Subcatchments.Subcatchment item);

    public abstract ISpatialObject CreateSpatialObject(Objects.Nodes.Node item);

    public abstract ISpatialObject CreateSpatialObject(Objects.Links.Link item);

    public abstract ISpatialClass CreateSpatialClass(Objects.ObjectKind objectKind, Objects.Attribute[] attributes);

    public abstract ISpatialDataset CreateSpatialDataset();

    public string ErrorMessage { get; set; }
  }
}
