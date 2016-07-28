//
//  main.swift
//  SimpleIcon
//
//  Created by Maurice on 08/07/16.
//  Copyright © 2016 Frobyte. All rights reserved.
//

import Foundation

do {
    let s = try SimpleIcon(file: "/Users/mandrakey/projects/SimpleIcon/swift/SimpleIcon/data/schwert.txt")
    s.display()
} catch {
    print("Failed to create simpleicon: \(error)")
}
