package com.ms.dxmedia;

import com.ms.dxmedia.rawcom.*;
import com.ms.com.*;

public class PickableImage {
  public PickableImage(ImageBvr im) {
      makeMe(im, false);
  }

  public PickableImage(ImageBvr im, boolean pickableWhenOccluded) {
      makeMe(im, pickableWhenOccluded);
  }
    
  public ImageBvr getImageBvr() { return _bvr; }

    // The event data generated by this event will be a PairBvr
    // of (Point2Bvr, Vector2Bvr)
  public DXMEvent getPickEvent() { return _event; }

  private void makeMe(ImageBvr im, boolean pickableWhenOccluded) {
      try {
	  IDAPickableResult res;
	  if (pickableWhenOccluded) {
	      res = im.getCOMPtr().PickableOccluded();
	  } else {
	      res = im.getCOMPtr().Pickable();
	  }
          _bvr = new ImageBvr(res.getImage());
          _event = new DXMEvent(res.getPickEvent());
      } catch (ComFailException e) {
          throw Statics.handleError(e);
      }      
  }
  private int _id;
  private DXMEvent _event;
  private ImageBvr _bvr;
}
