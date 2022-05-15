import math


class EuclideanDistTracker:
    def __init__(self):
        # Store the center positions of the objects
        self.center_points = {}
        # Keep the count of the IDs
        # each time a new object id detected, the count will increase by one
        self.id_count = 0


    def update(self, objects_rect, reset):
        # Objects boxes and ids
        objects_bbs_ids = []

        # Get center point of new object
        for rect in objects_rect:
            x, y, w, h = rect
            # calculate center point of this car
            cx = (x + x + w) // 2
            cy = (y + y + h) // 2

            # Find out if that object was detected already
            same_object_detected = False
            for id, pt in self.center_points.items(): 
                # Caculate distance between 2 center point
                dist = math.hypot(cx - pt[0], cy - pt[1])
                # if between is less than 25 is different car
                if dist < 25:
                    # set new center point for new car
                    self.center_points[id] = (cx, cy)
                    
                    # print(self.center_points)
                    # add car info into list
                    objects_bbs_ids.append([x, y, w, h, id])
                    same_object_detected = True
                    break

            # New object is detected we assign the ID to that object
            if same_object_detected is False:
                # create new center point for new car
                self.center_points[self.id_count] = (cx, cy)
                # add car info into list
                objects_bbs_ids.append([x, y, w, h, self.id_count])
                self.id_count += 1

        # Clean the dictionary by center points to remove IDS not used anymore
        
        # create new direction for current cars display
        new_center_points = {}
        for obj_bb_id in objects_bbs_ids:
            _, _, _, _, object_id = obj_bb_id
            center = self.center_points[object_id]
            new_center_points[object_id] = center

        # Update dictionary with IDs not used removed
        # replace old direction to new direction
        self.center_points = new_center_points.copy()
        if reset == True:
            self.id_count = 0
        return objects_bbs_ids



