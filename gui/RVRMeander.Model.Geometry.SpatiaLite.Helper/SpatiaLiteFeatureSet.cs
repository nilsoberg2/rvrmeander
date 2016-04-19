// Originally part of DotSpatial.Plugins.SpatiaLite, DotSpatial release 1.7.  Licensed under the LGPL.
using DotSpatial.Data;
using DotSpatial.Topology;

namespace RVRMeander.Model.Geometry.SpatiaLite.Helper
{
    public class SpatiaLiteFeatureSet : FeatureSet
    {
        public SpatiaLiteFeatureSet(FeatureType fType)
            :base(fType)
        {
        }

        public override IFeature GetFeature(int index)
        {
            var res =  base.GetFeature(index);
            if (res.DataRow == null)
            {
                res.DataRow = DataTable.Rows[index];
            }
            return res;
        }
    }
}