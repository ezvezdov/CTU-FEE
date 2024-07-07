from rosemary import Item, update

def test_is_name_constant():
    return_value = True
    names_quality = [['Bread',5],['Aged Brie',5],['Diamond',100],['Tickets',5]]
    for i in range(len(names_quality)):
        obj = Item(names_quality[i][0], days_left=3, quality=names_quality[0][1])
        update(obj)
        return_value *= (obj.name == names_quality[i][0])
    return bool(return_value)

def test_quality_borders():
    names_days_quality = [['Bread',4,48], ['Bread',3,2], ['Aged Brie',4,48],['Aged Brie',5,50],['Tickets',11,40],['Tickets',1,40]]
    return_value = True
    for i in range(len(names_days_quality)):
        item = Item(names_days_quality[i][0],names_days_quality[i][1],names_days_quality[i][2])
        for days in range(names_days_quality[i][1]):
            update(item)
            return_value *= (item.quality >= 0 and item.quality <= 50)
    return bool(return_value)

def test_days_change():
    names_days = ['Bread','Aged Brie','Tickets']
    return_value = True
    for i in range(len(names_days)):
        item = Item(names_days[i], days_left=6, quality=5)
        update(item)
        return_value *= (item.days_left == 5)
    return bool(return_value)

def test_default_quality_change():
    start_quality = 6
    item = Item('Milk',5,start_quality)
    update(item)
    return item.quality+1 == start_quality

def test_quality_of_diamond():
    return_value = True
    for days in range(20):
        diamond = Item('Diamond', days_left=3, quality=100)
        update(diamond)
        return_value *= (diamond.quality == 100)
    return bool(return_value)

def test_days_of_diamond():
    start_days = 3
    return_value = True
    for days in range(20):
        diamond = Item('Diamond', days_left=start_days, quality=100)
        update(diamond)
        return_value *= (diamond.days_left == start_days)
    return bool(return_value)

def test_cheese_quality():
    cheese = Item('Aged Brie', 4, 40)
    update(cheese)
    return (cheese.quality - 40 == 1)

def test_tickets_quality():
    start_quality = 10
    return_value = True
    for days in range(15):
        ticket = Item('Tickets', days, start_quality)
        update(ticket)
        return_value *= (days > 10 and ticket.quality - start_quality == 1 or days == 0 and ticket.quality == 0 or days >= 6 and days <= 10 and ticket.quality - start_quality == 2 or days >= 1 and days <= 5 and ticket.quality - start_quality == 3)
    return bool(return_value)

def test_fast_quality_change():
    start_quality = 6
    item = Item('Milk',0,start_quality)
    update(item)
    return (item.quality+2 == start_quality) 
