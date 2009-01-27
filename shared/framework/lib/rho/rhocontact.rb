module Rho
	class RhoContact
		class << self
			def find(param)
				pb = Phonebook::openPhonebook
				if pb.nil?
					puts "Can't open phonebook"
					return nil
				elsif param == :all or param == 'all'
					records = Phonebook::getallPhonebookRecords(pb)
					Phonebook::closePhonebook(pb)
					return records
				else
					record = Phonebook::getPhonebookRecord(pb,param)
					Phonebook::closePhonebook(pb)
					return record
				end
			end

			def create!(properties)
				pb = Phonebook::openPhonebook
				unless pb.nil?
					record = Phonebook::createRecord(pb)
					if record.nil?
						puts "Can't find record " + properties['id']
					else
						properties.each do |key,value|
							Phonebook::setRecordValue(record,key,value)
						end
						Phonebook::addRecord(pb,record)
					end
					Phonebook::closePhonebook(pb)
				end
			end

			def destroy(recordId)
				pb = Phonebook::openPhonebook
				unless pb.nil?
					record = Phonebook::openPhonebookRecord(pb,recordId)
					if record.nil?
						puts "Can't find record " + recordId
					else
						Phonebook::deleteRecord(pb,record)
					end
					Phonebook::closePhonebook(pb)
				end
			end

			def update_attributes(properties)
				pb = Phonebook::openPhonebook
				unless pb.nil?
					record = Phonebook::openPhonebookRecord(pb,properties['id'])
					if record.nil?
						puts "Can't find record " + properties['id']
					else
						properties.each do |key,value|
							Phonebook::setRecordValue(record,key,value)
						end
						Phonebook::saveRecord(pb,record)
					end
					Phonebook::closePhonebook(pb)
				end
			end

		end
	end
end